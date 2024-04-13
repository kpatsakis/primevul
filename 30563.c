_dbus_get_mutex_name (DBusString *out,const char *scope)
{
  return _dbus_get_address_string (out,cDBusDaemonMutex,scope);
}
