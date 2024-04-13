_dbus_win_error_from_last_error (void)
{
  switch (GetLastError())
    {
    case 0:
      return DBUS_ERROR_FAILED;
    
    case ERROR_NO_MORE_FILES:
    case ERROR_TOO_MANY_OPEN_FILES:
      return DBUS_ERROR_LIMITS_EXCEEDED; /* kernel out of memory */

    case ERROR_ACCESS_DENIED:
    case ERROR_CANNOT_MAKE:
      return DBUS_ERROR_ACCESS_DENIED;

    case ERROR_NOT_ENOUGH_MEMORY:
      return DBUS_ERROR_NO_MEMORY;

    case ERROR_FILE_EXISTS:
      return DBUS_ERROR_FILE_EXISTS;

    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
      return DBUS_ERROR_FILE_NOT_FOUND;
    }
  
  return DBUS_ERROR_FAILED;
}
