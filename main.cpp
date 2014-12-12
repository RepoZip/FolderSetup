#include "windows.h"
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <stdio.h>
#include <iostream>
#include "strsafe.h"

using namespace std;

HRESULT CreateLink(LPCWSTR lpszPathObj, LPCSTR lpszPathLink, LPCWSTR lpszDesc);

int main(int argc,char** argv)
{
	char *user,*file,*filename, *target;
	char *dfile = "\\Links\\RepoZip.lnk";
	char *comment = "crate favorite folder link for repozip folder";
	bool re = false;
	int tSize, fSize;
	size_t requiredSize;

	filename = "\\RepoZip";
	getenv_s(&requiredSize, NULL, 0, "USERPROFILE");
	if (requiredSize == 0)
	{
      printf("USERPROFILE doesn't exist!\n");
      exit(1);
	}

	user = (char*) malloc(requiredSize * sizeof(char));
	if (!user)
	{
      printf("Failed to allocate memory!\n");
      exit(1);
	}

	// Get the value of the LIB environment variable.
	getenv_s( &requiredSize, user, requiredSize, "USERPROFILE" );

	tSize = strlen(user)+strlen(dfile)+2;	
	fSize = strlen(user)+strlen(filename)+2;
	target = (char *)calloc(tSize,sizeof(char));
	file = (char *)calloc(fSize, sizeof(char));
	strcpy_s(target, tSize, user);
	strcat_s(target, tSize, dfile);
	strcpy_s(file, fSize, user);
	strcat_s(file, fSize, filename);
	
	re = CreateDirectory(file,NULL);
	if(!re){
		cout << "folder " << file << " creation fail"<< endl;
	}else{
		cout << "successfully create folder" << file << endl;
	}
	
	CoInitialize(NULL);
	HRESULT h = CreateLink((LPCWSTR)file, (LPCSTR)target,(LPCWSTR)comment);
	CoUninitialize();

	while(true);
	return 0;
}

HRESULT CreateLink(LPCWSTR lpszPathObj, LPCSTR lpszPathLink, LPCWSTR lpszDesc)
{ 
	HRESULT hres; 
	IShellLink* psl; 
 
	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called.
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl); 
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
