//main_functions.cpp
#include <SDL.h>
#include <Windows.h>
#include <vector>
#include <Shobjidl.h>
#include <algorithm>
#include <set>

#include "simple_renderables.h"
#include "Main_Functions.h"
#include "Vector2.h"
#include "Cursor.h"

Vector2 Main::getDisplayDims() {
	SDL_Rect r;
	SDL_GetDisplayBounds(0, &r);
	return {r.w, r.h};
}


void Main::updateRenderables() {
	Cursor::hasCursorChanged = false;
	SDL_RenderClear(Main::renderer);


	//objects are rendered in reverse order to ensure objects with higher priority are updated first but also rendered more forward 

	for (int i = Main::renderables.size() - 1; i >= 0; i--) {
		Renderable* r = Main::renderables[i];
		r->update();
	}
	for (Renderable* r : Main::renderables) if (!r->moveForwardWhenFocused || r != Cursor::focusedItem) r->render();


	if (Cursor::focusedItem != nullptr && Cursor::focusedItem->moveForwardWhenFocused) Cursor::focusedItem->render(); //render the item held by the cursor to the top of the screen if required

	SDL_RenderPresent(Main::renderer);
	//set cursor to normal if it isnt interacting with anything
	if (!Cursor::hasCursorChanged && Cursor::currentCursorType!=SDL_SYSTEM_CURSOR_ARROW) Cursor::setCursor(SDL_SYSTEM_CURSOR_ARROW);
}



std::vector<std::wstring> Main::openFileExplorerLoad(std::vector<std::pair<std::wstring,std::wstring>> allowedFiles, bool multiselect) {

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
	IShellItemArray* pItems;
	DWORD count;
	std::vector<std::wstring> items = {};
	if (fileEx->GetResults(&pItems) == S_OK) {
		pItems->GetCount(&count);
		for (int i = 0; i < count; i++) {
			PWSTR str = NULL;
			IShellItem* item;
			pItems->GetItemAt(i, &item);
			item->GetDisplayName(SIGDN_FILESYSPATH, &str);
			items.push_back(str);
			item->Release();
		}
	}

	//free file dialog and items
	pItems->Release();
	fileEx->Release();
	return items;


}


std::wstring Main::openFileExplorerSave(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles) {
	IFileSaveDialog* fileEx;
	wchar_t empty[] =L"";
	CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&fileEx));

	int numOfFileTypes = allowedFiles.size();
	COMDLG_FILTERSPEC* COMAllowedFiles = new COMDLG_FILTERSPEC[numOfFileTypes];
	for (int i = 0; i < numOfFileTypes; i++) COMAllowedFiles[i] = { allowedFiles[i].first.c_str(),allowedFiles[i].second.c_str() };
	fileEx->SetFileTypes(numOfFileTypes, COMAllowedFiles);
	delete COMAllowedFiles;

	fileEx->Show(NULL);
	IShellItem* fileName;
	fileEx->GetResult(&fileName);
	PWSTR fileNameBuf = empty;
	fileName->GetDisplayName(SIGDN_FILESYSPATH, &fileNameBuf);

	//free file dialog and items
	fileName->Release();
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
	if (datasrc == 0)std::cout << "bruh\n";
	HGLOBAL hresData= LoadResource(hModule, datasrc);
	void* data=LockResource(hresData);
	int dataSz=SizeofResource(hModule, datasrc);
	std::cout << dataSz << "\n";
	return { data,dataSz };
}


