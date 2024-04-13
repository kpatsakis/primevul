_dbus_append_user_from_current_process (DBusString *str)
{
  dbus_bool_t retval = FALSE;
  char *sid = NULL;

  if (!_dbus_getsid(&sid))
    return FALSE;

  retval = _dbus_string_append (str,sid);

  LocalFree(sid);
  return retval;
}
