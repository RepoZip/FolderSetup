#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include<stdio.h>
#include<iostream>
#include "strsafe.h"

using namespace std;

HRESULT CreateLink(LPCWSTR lpszPathObj, LPCSTR lpszPathLink, LPCWSTR lpszDesc);

int main(int argc,char** argv)
{
	char *file,*filename, *target;
	char *dfile = "\\Links\\RepoZip";
	char *comment = "crate favorite folder link for repozip folder";
	bool re;

	filename = "\\RepoZip";
	char *user = getenv("USERPROFILE");
	if(user!= NULL){
		target = (char *)calloc(strlen(user),strlen(dfile));
		file = (char *)calloc(strlen(user),strlen(filename));
		strcpy(target,user);
		strcat(target, dfile);
		strcpy(file,user);
		strcat(file,filename);
	}else{
		cout << "couldn't find user name" << endl;
	}

	re = CreateDirectory(file,NULL);
	if(!re){
		cout << "folder" << file << "creation fail"<< endl;
	}
	
	CoInitialize(NULL);
	HRESULT h = CreateLink((LPCWSTR)file, (LPCSTR)target,(LPCWSTR)comment);
	CoUninitialize();
	
	return 0;
}

HRESULT CreateLink(LPCWSTR lpszPathObj, LPCSTR lpszPathLink, LPCWSTR lpszDesc)
{ 
	HRESULT hres; 
	IShellLink* psl; 
 
	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_FolderShortcut, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
	if (SUCCEEDED(hres)) 
	{ 
		IPersistFile* ppf; 
 
		// Set the path to the shortcut target and add the description. 
		psl->SetPath((LPCSTR)lpszPathObj); 
		psl->SetDescription((LPCSTR)lpszDesc); 
 
		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
 
		if (SUCCEEDED(hres)) 
		{ 
			WCHAR wsz[MAX_PATH]; 
 
			// Ensure that the string is Unicode. 
			MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 
            
			// Add code here to check return value from MultiByteWideChar 
			// for success.
			cout << "success" << endl;
 
			// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(wsz, TRUE); 
			ppf->Release(); 
		} 
		psl->Release(); 
	} 
	return hres; 
}

/*

