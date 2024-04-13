_dbus_get_install_root(char *prefix, int len)
{
    DWORD pathLength;
    char *lastSlash;
    SetLastError( 0 );
    pathLength = GetModuleFileNameA(_dbus_win_get_dll_hmodule(), prefix, len);
    if ( pathLength == 0 || GetLastError() != 0 ) {
        *prefix = '\0';
        return FALSE;
    }
    lastSlash = _mbsrchr(prefix, '\\');
    if (lastSlash == NULL) {
        *prefix = '\0';
        return FALSE;
    }
    lastSlash[1] = 0;


    if (lastSlash - prefix >= 4 && strnicmp(lastSlash - 4, "\\bin", 4) == 0)
        lastSlash[-3] = 0;
    else if (lastSlash - prefix >= 10 && strnicmp(lastSlash - 10, "\\bin\\debug", 10) == 0)
        lastSlash[-9] = 0;
    else if (lastSlash - prefix >= 12 && strnicmp(lastSlash - 12, "\\bin\\release", 12) == 0)
        lastSlash[-11] = 0;

    return TRUE;
}
