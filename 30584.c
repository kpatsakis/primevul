_dbus_win_error_string (int error_number)
{
  char *msg;

  FormatMessageA (FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_IGNORE_INSERTS |
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, error_number, 0,
                  (LPSTR) &msg, 0, NULL);

  if (msg[strlen (msg) - 1] == '\n')
    msg[strlen (msg) - 1] = '\0';
  if (msg[strlen (msg) - 1] == '\r')
    msg[strlen (msg) - 1] = '\0';

  return msg;
}
