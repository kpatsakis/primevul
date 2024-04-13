_dbus_close_socket (int               fd,
                    DBusError        *error)
{
  return _dbus_close (fd, error);
}
