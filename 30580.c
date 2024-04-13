_dbus_set_fd_nonblocking (int             handle,
                          DBusError      *error)
{
  u_long one = 1;

  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  if (ioctlsocket (handle, FIONBIO, &one) == SOCKET_ERROR)
    {
      DBUS_SOCKET_SET_ERRNO ();
      dbus_set_error (error, _dbus_error_from_errno (errno),
                      "Failed to set socket %d:%d to nonblocking: %s", handle,
                      _dbus_strerror_from_errno ());
      return FALSE;
    }

  return TRUE;
}
