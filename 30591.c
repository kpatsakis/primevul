static DWORD WINAPI dump_thread_proc(LPVOID lpParameter)
{
    dump_backtrace_for_thread((HANDLE)lpParameter);
    return 0;
}
