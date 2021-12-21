#include "pch.h"
#include "dll.h"
#include <windows.h>
#include <stdio.h>
#include <aclapi.h>
#include <tchar.h>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
    //int result = _snprintf_s(buffer, dwBufferSize, 5, "golub");
    //*pdwBytesWritten = result;
    //return result;

    DWORD dwRtnCode = 0;
    PSID pSidOwner = NULL;
    BOOL bRtnBool = TRUE;
    LPTSTR AcctName = NULL;
    LPTSTR DomainName = NULL;
    DWORD dwAcctName = 1, dwDomainName = 1;
    SID_NAME_USE eUse = SidTypeUnknown;
    HANDLE hFile;
    PSECURITY_DESCRIPTOR pSD = NULL;


    // Get the handle of the file object.
    hFile = CreateFile(
        TEXT("dllmain.cpp"),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    // Check GetLastError for CreateFile error code.
    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD dwErrorCode = 0;

        dwErrorCode = GetLastError();
        _tprintf(TEXT("CreateFile error = %d\n"), dwErrorCode);
        return -1;
    }

    // Get the owner SID of the file.
    dwRtnCode = GetSecurityInfo(
        hFile,
        SE_FILE_OBJECT,
        OWNER_SECURITY_INFORMATION,
        &pSidOwner,
        NULL,
        NULL,
        NULL,
        &pSD);

    // Check GetLastError for GetSecurityInfo error condition.
    if (dwRtnCode != ERROR_SUCCESS) {
        DWORD dwErrorCode = 0;

        dwErrorCode = GetLastError();
        _tprintf(TEXT("GetSecurityInfo error = %d\n"), dwErrorCode);
        return -1;
    }

    // First call to LookupAccountSid to get the buffer sizes.
    bRtnBool = LookupAccountSid(
        NULL,           // local computer
        pSidOwner,
        AcctName,
        (LPDWORD)&dwAcctName,
        DomainName,
        (LPDWORD)&dwDomainName,
        &eUse);

    // Reallocate memory for the buffers.
    AcctName = (LPTSTR)GlobalAlloc(
        GMEM_FIXED,
        dwAcctName);

    // Check GetLastError for GlobalAlloc error condition.
    if (AcctName == NULL) {
        DWORD dwErrorCode = 0;

        dwErrorCode = GetLastError();
        _tprintf(TEXT("GlobalAlloc error = %d\n"), dwErrorCode);
        return -1;
    }

    DomainName = (LPTSTR)GlobalAlloc(
        GMEM_FIXED,
        dwDomainName);

    // Check GetLastError for GlobalAlloc error condition.
    if (DomainName == NULL) {
        DWORD dwErrorCode = 0;

        dwErrorCode = GetLastError();
        _tprintf(TEXT("GlobalAlloc error = %d\n"), dwErrorCode);
        return -1;

    }

    // Second call to LookupAccountSid to get the account name.
    bRtnBool = LookupAccountSid(
        NULL,                   // name of local or remote computer
        pSidOwner,              // security identifier
        AcctName,               // account name buffer
        (LPDWORD)&dwAcctName,   // size of account name buffer 
        DomainName,             // domain name
        (LPDWORD)&dwDomainName, // size of domain name buffer
        &eUse);                 // SID type

  // Check GetLastError for LookupAccountSid error condition.
    if (bRtnBool == FALSE) {
        DWORD dwErrorCode = 0;

        dwErrorCode = GetLastError();

        if (dwErrorCode == ERROR_NONE_MAPPED)
            _tprintf(TEXT
            ("Account owner not found for specified SID.\n"));
        else
            _tprintf(TEXT("Error in LookupAccountSid.\n"));
        return -1;

    }
    else if (bRtnBool == TRUE)

        // Print the account name.
        _tprintf(TEXT("Account owner = %s\n"), AcctName);
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
    int result = _snprintf_s(buffer, dwBufferSize, 15, "some useless s-");
    *pdwBytesWritten = result;
    return result;
}

VOID Execute()
{
    SYSTEM_INFO siSysInfo;

    // Copy the hardware information to the SYSTEM_INFO structure. 

    GetSystemInfo(&siSysInfo);

    // Display the contents of the SYSTEM_INFO structure. 

    printf("Hardware information: \n");
    printf("  OEM ID: %u\n", siSysInfo.dwOemId);
    printf("  Number of processors: %u\n",
        siSysInfo.dwNumberOfProcessors);
    printf("  Page size: %u\n", siSysInfo.dwPageSize);
    printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
    printf("  Minimum application address: %lx\n",
        siSysInfo.lpMinimumApplicationAddress);
    printf("  Maximum application address: %lx\n",
        siSysInfo.lpMaximumApplicationAddress);
    printf("  Active processor mask: %u\n",
        siSysInfo.dwActiveProcessorMask);
}