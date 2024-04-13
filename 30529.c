_dbus_accept  (int listen_fd)
{
  int client_fd;

 retry:
  client_fd = accept (listen_fd, NULL, NULL);

  if (DBUS_SOCKET_IS_INVALID (client_fd))
    {
      DBUS_SOCKET_SET_ERRNO ();
      if (errno == EINTR)
        goto retry;
    }

  _dbus_verbose ("client fd %d accepted\n", client_fd);
  
  return client_fd;
}
