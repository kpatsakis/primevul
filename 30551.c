_dbus_file_exists (const char *file)
{
  DWORD attributes = GetFileAttributesA (file);

  if (attributes != INVALID_FILE_ATTRIBUTES && GetLastError() != ERROR_PATH_NOT_FOUND)
    return TRUE;
  else
    return FALSE;  
}
