#include <windows.h>
#include <stdio.h>

int main(void) {

  LPCTSTR pipename = "\\\\.\\pipe\\mynamedpipe";
  HANDLE hNamedPipe; // дескриптор канала
  DWORD szAddress; // адрес выделенной памяти
  DWORD cbRead, cbWritten; // буфферы (имеют промежуточное значение)
  char* msg; // получаемая строка
  BOOL Success; // флаг успешного открытия/считывания/записи

  printf("creating named pipe...");
  hNamedPipe = CreateNamedPipe(
      pipename,
      PIPE_ACCESS_DUPLEX, // канал двунаправленный; серверные и клиентские процессы могут читать с канала и записывать в него
      PIPE_TYPE_MESSAGE | // данные записываются в канал и считываются из него как поток сообщений
          PIPE_READMODE_MESSAGE |
          PIPE_WAIT, // неограниченное ожидание в течение выполнения действий клиентским процессом
      PIPE_UNLIMITED_INSTANCES, // максимальное число экземпляров
      sizeof(DWORD), // резерв для выводимого буфера
      sizeof(DWORD), // резерв для вводимого буфера
      100,
      NULL);
  if (hNamedPipe == INVALID_HANDLE_VALUE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nconnecting to pipe...");
  ConnectNamedPipe(hNamedPipe, NULL);

  printf("success!\nreading from pipe...");
  Success = ReadFile(
      hNamedPipe,
      &szAddress, // буфер для считывания
      sizeof(szAddress), // объём считываемых данных
      &cbRead,
      NULL);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nyour string: ");
  msg = (char*)szAddress;
  printf_s("%s\n", msg);

  printf("sending answer to proccessA...");
  Success = WriteFile(
      hNamedPipe,
      &szAddress, // данные для записи
      sizeof(szAddress), // объём записываемых данных
      &cbWritten,
      NULL);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\ndisconnecting from named pipe...");
  Success = DisconnectNamedPipe(hNamedPipe);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nclosing named pipe...");
  Success = CloseHandle(hNamedPipe);
  if (Success == FALSE) {
    printf("failure:(\n");
    system("pause");
    return -1;
  }

  printf("success!\nprocessB was finished...\n");
  system("pause");
  return 0;
}