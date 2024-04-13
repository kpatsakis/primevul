_dbus_get_shm_name (DBusString *out,const char *scope)
{
  return _dbus_get_address_string (out,cDBusDaemonAddressInfo,scope);
}
