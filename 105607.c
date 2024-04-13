throw_error (DBusGMethodInvocation *context,
             int error_code,
             const char *format,
             ...)
{
  GError *error;
  va_list args;
  char *message;

  if (context == NULL)
    return TRUE;

  va_start (args, format);
  message = g_strdup_vprintf (format, args);
  va_end (args);

  error = g_error_new (ERROR, error_code, "%s", message);
  dbus_g_method_return_error (context, error);
  g_error_free (error);
  g_free (message);
  return TRUE;
}
