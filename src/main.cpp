#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>

using namespace std;
const wchar_t *GetWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}


int GetProcId(char* ProcName)
{
	PROCESSENTRY32   pe32;
	HANDLE         hSnapshot = NULL;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    bool issame;

	if (Process32First(hSnapshot, &pe32))
	{
		do {

    
            if(strlen(pe32.szExeFile) == strlen(ProcName)){
            issame = true;
                for(int i=0; i< strlen(ProcName); i++){
                    cout << ProcName[i] << " " << pe32.szExeFile[i] << endl;
                    if (ProcName[i] == pe32.szExeFile[i]){
                        continue;
                    }else{
                        issame = false;
                        break;
                    }
                }
            }
            if(issame == true){
                break;
            }
		} while (Process32Next(hSnapshot, &pe32));
	}

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);

	if (issame == true) {
		return pe32.th32ProcessID;
	}
	else {
		return -1;
	}
}


int main()
{
	LPVOID addr = (LPVOID)0x004B06BB;
	char name[] = "game.exe";


	int pid = GetProcId(name);
	if (pid == -1) {
		printf("Cannot found the game.\n");
		return 0 ;
	}

	printf("Process Found. PID is %d \n", pid);

	HANDLE handle = OpenProcess(MAXIMUM_ALLOWED, false, pid);
	BYTE jmpcode[2] = { 0x0F,0x84 };
	SIZE_T* r =0;
	if (WriteProcessMemory(handle, addr, jmpcode, 2, r)) {
		printf("Success! \n");
	}
	else {
		printf("Write process memory failed \n");
	}
	
	printf("Enter any key ...");
	getchar();
	return 0;
}


