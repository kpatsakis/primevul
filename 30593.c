static BOOL init_backtrace()
{
    HMODULE hmodDbgHelp = LoadLibraryA("dbghelp");
/*
    #define GETFUNC(x) \
    p##x = (typeof(x)*)GetProcAddress(hmodDbgHelp, #x); \
    if (!p##x) \
    { \
        return FALSE; \
    }
    */



#define FUNC(x) #x

      pStackWalk = (BOOL  (WINAPI *)(
DWORD MachineType,
HANDLE hProcess,
HANDLE hThread,
LPSTACKFRAME StackFrame,
PVOID ContextRecord,
PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine,
PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine,
PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine,
PTRANSLATE_ADDRESS_ROUTINE TranslateAddress
))GetProcAddress (hmodDbgHelp, FUNC(StackWalk));
#ifdef _WIN64
    pSymGetModuleBase=(DWORD64  (WINAPI *)(
  HANDLE hProcess,
  DWORD64 dwAddr
))GetProcAddress (hmodDbgHelp, FUNC(SymGetModuleBase));
    pSymFunctionTableAccess=(PVOID  (WINAPI *)(
  HANDLE hProcess,
  DWORD64 AddrBase
))GetProcAddress (hmodDbgHelp, FUNC(SymFunctionTableAccess));
#else
    pSymGetModuleBase=(DWORD  (WINAPI *)(
  HANDLE hProcess,
  DWORD dwAddr
))GetProcAddress (hmodDbgHelp, FUNC(SymGetModuleBase));
    pSymFunctionTableAccess=(PVOID  (WINAPI *)(
  HANDLE hProcess,
  DWORD AddrBase
))GetProcAddress (hmodDbgHelp, FUNC(SymFunctionTableAccess));
#endif
    pSymInitialize = (BOOL  (WINAPI *)(
  HANDLE hProcess,
  PSTR UserSearchPath,
  BOOL fInvadeProcess
))GetProcAddress (hmodDbgHelp, FUNC(SymInitialize));
    pSymGetSymFromAddr = (BOOL  (WINAPI *)(
  HANDLE hProcess,
  DWORD Address,
  PDWORD Displacement,
  PIMAGEHLP_SYMBOL Symbol
))GetProcAddress (hmodDbgHelp, FUNC(SymGetSymFromAddr));
    pSymGetModuleInfo = (BOOL  (WINAPI *)(
  HANDLE hProcess,
  DWORD dwAddr,
  PIMAGEHLP_MODULE ModuleInfo
))GetProcAddress (hmodDbgHelp, FUNC(SymGetModuleInfo));
pSymSetOptions = (DWORD  (WINAPI *)(
DWORD SymOptions
))GetProcAddress (hmodDbgHelp, FUNC(SymSetOptions));


    pSymSetOptions(SYMOPT_UNDNAME);

    pSymInitialize(GetCurrentProcess(), NULL, TRUE);

    return TRUE;
}
