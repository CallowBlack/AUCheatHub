#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <tchar.h>
using namespace std;

// Find Amoung Us process id by name.
DWORD FindAUProccess();

// Simple LibraryA injection
bool inject(DWORD procId, LPSTR dllPath);

int main(int argc, char* argv[], char* envp[]) {
	char* path;
	if (argc == 1) {
		cout << "Input path to CheatDll file: ";
		path = new char[MAX_PATH];
		cin.getline(path, MAX_PATH);
		cin.ignore();
	}
	else {
		path = argv[1];
	}
	
	DWORD procid;
	while (true) {
		char ch;
		if (argc < 3 || !strcmp(argv[2], "none")) {
			cout << "Find Amoung Us process automatically ([y],[n])";
			cin.clear();
			cin >> ch;
			cout << endl;
		}
		else {
			if (!strcmp(argv[2], "auto"))
				ch = 'y';
			else
				ch = 'n';
		}

		if (ch == 'y' || ch == 'Y') {
			procid = FindAUProccess();
			if (procid == NULL) {
				cout << "Amoung Us process hasn't found. Try again or input process id manual." << endl;
				if (argc > 2 && !strcmp(argv[2], "auto"))
					argv[2] = (char*)"none";
				continue;
			}
			break;
		}
		else if (ch == 'n' || ch == 'N') {
			cout << "Input Amoung Us process id: "; cin.ignore();
			cin >> procid;
			cout << endl;

			HANDLE process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, procid);
			if (process == NULL) {
				cout << "Failed open process with error: " << GetLastError() << endl;
				if (argc > 2 && !strcmp(argv[2], "manual"))
					argv[2] = (char*)"none";
				continue;
			}
			CloseHandle(process);
			break;
		}
	}
	
	cout << "Injection..." << endl;
	if (inject(procid, path))
		cout << "Successfully injected." << endl;
	else
		cout << "Failed injection with error: " << GetLastError() << endl;
	
}

DWORD FindAUProccess() {
	auto procSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (procSnap == INVALID_HANDLE_VALUE) {
		return NULL;
	}

	PROCESSENTRY32 procData = { 0 };
	procData.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(procSnap, &procData))
	{
		cout << TEXT("Process32First") << endl;
		CloseHandle(procSnap);
		return NULL;
	}

	do {
		if (!lstrcmp(procData.szExeFile, L"Among Us.exe")) {
			CloseHandle(procSnap);
			return procData.th32ProcessID;
		}
	} while (Process32Next(procSnap, &procData));
	
	CloseHandle(procSnap);
	return NULL;
}


bool inject(DWORD procId, LPSTR dllPath) 
{
	HANDLE process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, procId);
	if (process == NULL)
		return false;

	auto kernelModule = GetModuleHandle(L"kernel32.dll");
	if (kernelModule == NULL)
		return false;
	LPVOID libraryAPtr = (LPVOID)GetProcAddress(kernelModule, "LoadLibraryA");

	LPVOID dllPathPtr = (LPVOID)VirtualAllocEx(process, NULL, strlen(dllPath) + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (dllPathPtr == NULL)
		return false;

	WriteProcessMemory(process, (LPVOID)dllPathPtr, dllPath, strlen(dllPath) + 1, NULL);
	HANDLE thread = CreateRemoteThread(process, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, (LPVOID)dllPathPtr, NULL, NULL);
	CloseHandle(process);
	
	return thread != NULL;
}