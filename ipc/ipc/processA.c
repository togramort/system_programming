#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

#define MAXLENGTH 101
DWORD GetProcessID(char*);

int main(void) {
  LPCTSTR pipename = "\\\\.\\pipe\\mynamedpipe";
  HANDLE hProcessB;
  LPVOID szAddress; // адрес в памяти процесса B
  DWORD cbWritten, cbRead, dBuf; // буфферы (имеют промежуточное значение)
  BOOL Success; // флаг успешного открытия/считывания/записи
  DWORD szSize = sizeof(char) * MAXLENGTH;
  char *str = (char*)malloc(szSize);
  HANDLE hPipe = INVALID_HANDLE_VALUE; // дескриптор канала

  printf("enter your string: ");
  gets_s(str, MAXLENGTH - 1);

  printf("opening processB...");
  hProcessB = OpenProcess(
      PROCESS_ALL_ACCESS, // устанавливает уровень доступа к объекту процесса
      FALSE, // параметр дескриптора наследования
      GetProcessID("ipc2.exe")); // ID процесса B
  if (hProcessB == INVALID_HANDLE_VALUE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nallocating memory for processB...");
  szAddress = VirtualAllocEx(
      hProcessB, // дескриптор процесса B
      0, // система сама решает, где выделять память
      szSize, // объём резервируемой памяти
      MEM_RESERVE | // память зарезервирована и освобождена
          MEM_COMMIT,
      PAGE_EXECUTE_READWRITE);

  printf("success!\nwriting your string in processB memory...");
  Success = WriteProcessMemory(
      hProcessB, // дескриптор процесса B
      szAddress, // куда записывать
      str, // что записывать
      szSize, // объём записываемых данных
      0);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nconnecting to named pipe...");
  while (hPipe == INVALID_HANDLE_VALUE) {
    hPipe = CreateFile(
        pipename, // имя именованного канала
        GENERIC_READ | // требуем доступ к чтению и записи
            GENERIC_WRITE,
        0, // без совместного доступа
        NULL, // без дескрипторов защиты
        OPEN_EXISTING, // открываем существующий канал
        0, // атрибуты файла по умолчанию
        NULL); // без дескриптора файла шаблона
  }

  printf("success!\nwriting memory address...");
  dBuf = (DWORD)szAddress;
  Success = WriteFile(
      hPipe, // дескриптор канала
      &dBuf, // данные для записи адрес памяти в процессе B
      sizeof(dBuf), // объём записываемых данных
      &cbWritten,
      NULL);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nwaiting answer from ProcessB...");
  Success = ReadFile(
      hPipe, // дескриптор канала
      &szAddress, // буфер для считывания
      sizeof(szAddress), // объём считываемых данных
      &cbRead,
      NULL);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nreleasing ProcessB memory...");
  Success = VirtualFreeEx(
      hProcessB,
      szAddress,
      0,
      MEM_RELEASE);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nclosing named pipe...");
  Success = CloseHandle(hPipe);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nclosing processB...");
  Success = CloseHandle(hProcessB);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nprocessA was finished...\n");
  system("pause");
  return 0;
}

DWORD GetProcessID(char* process) {
  PROCESSENTRY32 processEntry;
  DWORD processID = 0;
  HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if (Snapshot != INVALID_HANDLE_VALUE) {
    processEntry.dwSize = sizeof(processEntry);
    Process32First(Snapshot, &processEntry);

    while (Process32Next(Snapshot, &processEntry)) {
      if (strcmp(processEntry.szExeFile, process) == 0) {
        processID = processEntry.th32ProcessID;
        break;
      }
    }

    CloseHandle(Snapshot);
  }

  return processID;
}