//main_functions.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <SDL.h>
#include <Windows.h>
#include <vector>
#include <Shobjidl.h>
#include <algorithm>
#include <set>

#include "simple_renderables.h"
#include "Main_Functions.h"
#include "Vector2.h"
#include "task_objects.h"
#include "Cursor.h"

Vector2 Main::getDisplayDims() {
	SDL_Rect r;
	SDL_GetDisplayBounds(0, &r);
	return { r.w, r.h };
}


void Main::updateRenderables() {

	Cursor::hasCursorChanged = false;
	Main::clearScreenDrawBackground();


	//objects are rendered in reverse order to ensure objects with higher priority are updated first but also rendered more forward 

	for (int i = Main::renderables.size() - 1; i >= 0; i--) {
		Renderable* r = Main::renderables[i];
		r->update();
	}
	for (Renderable* r : Main::renderables) if (!r->moveForwardWhenFocused || r != Cursor::focusedItem) r->render();


	if (Cursor::focusedItem != nullptr && Cursor::focusedItem->moveForwardWhenFocused) Cursor::focusedItem->render(); //render the item held by the cursor to the top of the screen if required
	SDL_RenderPresent(Main::renderer);
	//set cursor to normal if it isnt interacting with anything
	if (!Cursor::hasCursorChanged && Cursor::currentCursorType != SDL_SYSTEM_CURSOR_ARROW) Cursor::setCursor(SDL_SYSTEM_CURSOR_ARROW);
}



std::wstring Main::openFileExplorerLoad(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles, bool multiselect) {

	IFileOpenDialog* fileEx;
	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&fileEx));
	IFileDialogEvents* e = nullptr;
	PWSTR f = NULL;
	//fileEx->Advise(NULL, &cookie);

	//set allowed file types
	int numOfFileTypes = allowedFiles.size();
	COMDLG_FILTERSPEC* COMAllowedFiles = new COMDLG_FILTERSPEC[numOfFileTypes];
	for (int i = 0; i < numOfFileTypes; i++) COMAllowedFiles[i] = { allowedFiles[i].first.c_str(),allowedFiles[i].second.c_str() };
	fileEx->SetFileTypes(numOfFileTypes, COMAllowedFiles);
	delete COMAllowedFiles;

	FILEOPENDIALOGOPTIONS options;
	fileEx->GetOptions(&options);
	if (multiselect) options |= FOS_ALLOWMULTISELECT;
	fileEx->SetOptions(options | options);

	fileEx->Show(NULL);
	wchar_t empty[] = L"";
	std::wstring text = L"";
	IShellItem* pItem;
	DWORD count;
	if (fileEx->GetResult(&pItem) == S_OK) {
		PWSTR str = NULL;
		pItem->GetDisplayName(SIGDN_FILESYSPATH, &str);
		pItem->Release();
		text = str;
	}

	fileEx->Release();
	return text;


}


std::wstring Main::openFileExplorerSave(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles) {
	IFileSaveDialog* fileEx;
	wchar_t empty[] = L"";
	CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&fileEx));

	int numOfFileTypes = allowedFiles.size();
	COMDLG_FILTERSPEC* COMAllowedFiles = new COMDLG_FILTERSPEC[numOfFileTypes];
	for (int i = 0; i < numOfFileTypes; i++) COMAllowedFiles[i] = { allowedFiles[i].first.c_str(),allowedFiles[i].second.c_str() };
	fileEx->SetFileTypes(numOfFileTypes, COMAllowedFiles);
	if (allowedFiles.size()==1) fileEx->SetDefaultExtension(allowedFiles[0].second.c_str());
	delete COMAllowedFiles;
	
	fileEx->Show(NULL);
	IShellItem* fileName = nullptr;
	fileEx->GetResult(&fileName);
	PWSTR fileNameBuf = empty;
	if (fileName != nullptr) {
		fileName->GetDisplayName(SIGDN_FILESYSPATH, &fileNameBuf);
		fileName->Release();
	}
	fileEx->Release();
	return fileNameBuf;
}

void Main::handleEvents(SDL_Event& e) {
	Main::textInputThisFrame = "";
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			SDL_Quit();
			Main::running = false;
			break;

		case (SDL_TEXTINPUT):
			Main::textInputThisFrame += e.text.text;
		}

	}
}

void Main::removeDestroyedObjects() {
	std::set<Renderable*> objsToDelete = {};
	for (Renderable* obj : Main::renderables) {
		if (obj->toBeDestroyed()) objsToDelete.insert(obj);
	}
	for (Renderable* obj : objsToDelete) {
		auto it = std::find(Main::renderables.begin(), Main::renderables.end(), obj);
		*it = nullptr;
		if (it != Main::renderables.end()) Main::renderables.erase(it);
		delete obj;

	}
}

WindowsResource Main::loadWindowsResource(int id) {
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC datasrc = FindResource(hModule, MAKEINTRESOURCE(id), TEXT("FONT"));
	HGLOBAL hresData = LoadResource(hModule, datasrc);
	void* data = LockResource(hresData);
	int dataSz = SizeofResource(hModule, datasrc);
	return { data,dataSz };
}


void Main::clearScreenDrawBackground() {
	Uint8 r, g, b, a;
	SDL_Color color = Main::bgColor;
	SDL_GetRenderDrawColor(Main::renderer, &r, &g, &b,&a);
	SDL_SetRenderDrawColor(Main::renderer, color.r, color.b, color.g, color.a);
	SDL_RenderClear(Main::renderer);
	SDL_SetRenderDrawColor(Main::renderer, r, g, b, a);
}

int Main::windowsErrMessageBoxOkCancel(std::string title, std::string msg) {
	return MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_ICONERROR |MB_YESNO);
}

int Main::windowsErrMessageBoxOk(std::string title, std::string msg) {
	return MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
}

std::string Main::getWindowsErrorMsg(long code) {
	DWORD errCode = (DWORD)code;
	LPSTR buffer = nullptr;

	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&buffer),
		0,
		NULL
	);

	std::string str = buffer;
	LocalFree(buffer);
	return str;
}

std::time_t Main::strTimeToTime(std::string strTime) {
	if (strTime.size() != 4) return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	int hours = std::stoi(strTime.substr(0, 2));
	int minutes = std::stoi(strTime.substr(2, 2));
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm tm = *std::localtime(&now);
	tm.tm_isdst = 1;
	tm.tm_hour = hours;
	tm.tm_min = minutes;
	tm.tm_sec = 0;
	std::time_t time = std::mktime(&tm);
	if (time < now) {
		tm.tm_mday++;
		time=std::mktime(&tm);
	}

	return time;
}


void Main::handleProgramArgs(std::vector<std::string> args,TaskList* tasklist) {
	bool executeImportedTasks = false;
	for (int i = 0; i < args.size(); i++) {
		if (args[i] == "--import_taskfile" && i + 1 < args.size()) tasklist->importTaskListFromPath(Utils::stringTowstring(args[i + 1]));
		if (args[i] == "--execute") executeImportedTasks = true;
	}
	if (executeImportedTasks) tasklist->executeTasks();
}

