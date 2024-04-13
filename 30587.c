_dbus_win_set_error_from_win_error (DBusError *error,
                                    int        code)
{
  char *msg;

  /* As we want the English message, use the A API */
  FormatMessageA (FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_IGNORE_INSERTS |
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, code, MAKELANGID (LANG_ENGLISH, SUBLANG_ENGLISH_US),
                  (LPSTR) &msg, 0, NULL);
  if (msg)
    {
      char *msg_copy;

      msg_copy = dbus_malloc (strlen (msg));
      strcpy (msg_copy, msg);
      LocalFree (msg);

      dbus_set_error (error, "win32.error", "%s", msg_copy);
    }
  else
    dbus_set_error (error, "win32.error", "Unknown error code %d or FormatMessage failed", code);
}
