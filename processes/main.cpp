#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

//solved 25 oct
//forward declarations
BOOL GetProcessList();
BOOL ListProcessModules(DWORD dwPID);
BOOL ListProcessThreads(DWORD dwOwnerPID);
int cntModule = 0;

int main() {
  GetProcessList();
  return 0;
}

BOOL GetProcessList() {
  HANDLE hProcessSnap;  //handle - descriptor - address
  HANDLE hProcess;
  PROCESSENTRY32 pe32; //entry in the list of processes

  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //take a snapshot of all processes in the system
  if (hProcessSnap == INVALID_HANDLE_VALUE) {
    return (FALSE); //if sth wrong with descriptor returns 0
  }

  pe32.dwSize = sizeof(PROCESSENTRY32); //set the size of the structure before using it

  if (!Process32First(hProcessSnap, &pe32)) { //extract information about the first process
    CloseHandle(hProcessSnap);
    return (FALSE);
  }

  //now walk the snapshot of processes, and display information about each process in turn
  while (Process32Next(hProcessSnap, &pe32)) {
    _tprintf(TEXT("\n\n====================================================="));
    _tprintf(TEXT("\nPROCESS NAME:  %s"), pe32.szExeFile);
    _tprintf(TEXT("\n-------------------------------------------------------"));

    //extract the priority class
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); //opens an existing local process object
    if (hProcess == INVALID_HANDLE_VALUE) {
      return (FALSE);
    }

    CloseHandle(hProcess);

    _tprintf(TEXT("\n  Process ID        = 0x%08lX"), pe32.th32ProcessID);
    _tprintf(TEXT("\n  Parent process ID = 0x%08lX"), pe32.th32ParentProcessID);
    _tprintf(TEXT("\n  Priority base     = %ld"), pe32.pcPriClassBase);

    //list the modules and threads associated with this process
    cntModule = 0;
    ListProcessModules(pe32.th32ProcessID);
    _tprintf(TEXT("\n  Module count      = %d"), cntModule);
    _tprintf(TEXT("\n-------------------------------------------------------"));
    ListProcessThreads(pe32.th32ProcessID);
    _tprintf(TEXT("\n  Thread count      = %lu"), pe32.cntThreads);
  }

  CloseHandle(hProcessSnap);
  return (TRUE);
}

BOOL ListProcessModules(DWORD dwPID) {
  HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
  MODULEENTRY32 me32;

  //take a snapshot of all modules in the specified process
  hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
  if (hModuleSnap == INVALID_HANDLE_VALUE) {
    return (FALSE);
  }

  me32.dwSize = sizeof(MODULEENTRY32); //set the size of the structure before using it

  //extract information about the first module and exit if unsuccessful
  if (!Module32First(hModuleSnap, &me32)) {
    CloseHandle(hModuleSnap);
    return (FALSE);
  }

  //display information about each module
  while (Module32Next(hModuleSnap, &me32)) {
    _tprintf(TEXT("\n\n     Path           = %s"), me32.szExePath);
    _tprintf(TEXT("\n     Base size      = %lu"), me32.modBaseSize);
    _tprintf(TEXT("\n"));
    ++cntModule;
  }

  CloseHandle(hModuleSnap);
  return (TRUE);
}

BOOL ListProcessThreads(DWORD dwOwnerPID) {
  HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
  THREADENTRY32 te32;

  //take a snapshot of all running threads
  hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
  if (hThreadSnap == INVALID_HANDLE_VALUE) {
    return (FALSE);
  }

  te32.dwSize = sizeof(THREADENTRY32); //fill in the size of the structure before using it

  //extract information about the first thread and exit if unsuccessful
  if (!Thread32First(hThreadSnap, &te32)) {
    CloseHandle(hThreadSnap);
    return (FALSE);
  }

  //display information about each thread
  while (Thread32Next(hThreadSnap, &te32)) {
    if (te32.th32OwnerProcessID == dwOwnerPID) {
      _tprintf(TEXT("\n\n     THREAD ID      = 0x%08lX"), te32.th32ThreadID);
      _tprintf(TEXT("\n     Base priority  = %ld"), te32.tpBasePri); //the base kernel priority level assigned to a thread (from 0 to 35)
      _tprintf(TEXT("\n"));
    }
  }

  CloseHandle(hThreadSnap);
  return (TRUE);
}