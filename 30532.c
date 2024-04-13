_dbus_append_system_config_file (DBusString *str)
{
  return _dbus_get_config_file_name(str, "system.conf");
}
