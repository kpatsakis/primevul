_dbus_accept  (int listen_fd)
{
  int client_fd;
  struct sockaddr addr;
  socklen_t addrlen;
#ifdef HAVE_ACCEPT4
  dbus_bool_t cloexec_done;
#endif

  addrlen = sizeof (addr);

 retry:

#ifdef HAVE_ACCEPT4
  /* We assume that if accept4 is available SOCK_CLOEXEC is too */
  client_fd = accept4 (listen_fd, &addr, &addrlen, SOCK_CLOEXEC);
  cloexec_done = client_fd >= 0;

  if (client_fd < 0 && errno == ENOSYS)
#endif
    {
      client_fd = accept (listen_fd, &addr, &addrlen);
    }

  if (client_fd < 0)
    {
      if (errno == EINTR)
        goto retry;
    }

  _dbus_verbose ("client fd %d accepted\n", client_fd);

#ifdef HAVE_ACCEPT4
  if (!cloexec_done)
#endif
    {
      _dbus_fd_set_close_on_exec(client_fd);
    }

  return client_fd;
}
