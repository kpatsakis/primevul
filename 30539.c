_dbus_close_socket (int        fd,
                    DBusError *error)
{
  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

 again:
  if (closesocket (fd) == SOCKET_ERROR)
    {
      DBUS_SOCKET_SET_ERRNO ();
      
      if (errno == EINTR)
        goto again;
        
      dbus_set_error (error, _dbus_error_from_errno (errno),
                      "Could not close socket: socket=%d, , %s",
                      fd, _dbus_strerror_from_errno ());
      return FALSE;
    }
  _dbus_verbose ("_dbus_close_socket: socket=%d, \n", fd);

  return TRUE;
}
