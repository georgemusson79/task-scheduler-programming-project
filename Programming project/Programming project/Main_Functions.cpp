//main_functions.cpp

#include "Main_Functions.h"
#include "Vector2.h"
#include <SDL.h>
#include <Windows.h>
#include <vector>
#include <Shobjidl.h>
Vector2 Main::getDisplayDims() {
	SDL_Rect r;
	SDL_GetDisplayBounds(0, &r);
	return {r.w, r.h};
}


void Main::updateRenderables() {
	SDL_RenderClear(Main::renderer);
	for (Renderable* r : Main::renderables) {
		r->render();
		r->update();
	}
	SDL_RenderPresent(Main::renderer);
}



std::vector<std::wstring> Main::openFileExplorerLoad(std::vector<std::pair<std::wstring,std::wstring>> allowedFiles, bool multiselect) {
	

	IFileOpenDialog* fileEx;
	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC, IID_PPV_ARGS(&fileEx));
	IFileDialogEvents* e = nullptr;
	DWORD cookie;
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
		}
	}

	return items;


}


std::wstring Main::openFileExplorerSave(std::vector<std::pair<std::wstring, std::wstring>> allowedFiles) {
	IFileSaveDialog* fileEx;
	wchar_t empty[] = L"";
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
	return fileNameBuf;
}

void Main::handleEvents(SDL_Event& e) {
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			SDL_Quit();
			Main::running = false;
		}
	}
}