static void dump_backtrace()
{
    HANDLE hCurrentThread;
    HANDLE hThread;
    DWORD dwThreadId;
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
        GetCurrentProcess(), &hCurrentThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
    hThread = CreateThread(NULL, 0, dump_thread_proc, (LPVOID)hCurrentThread,
        0, &dwThreadId);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    CloseHandle(hCurrentThread);
}
