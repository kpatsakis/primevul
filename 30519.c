_dbus_listen_tcp_socket (const char     *host,
                         const char     *port,
                         const char     *family,
                         DBusString     *retport,
                         int           **fds_p,
                         DBusError      *error)
{
  int saved_errno;
  int nlisten_fd = 0, *listen_fd = NULL, res, i;
  struct addrinfo hints;
  struct addrinfo *ai, *tmp;
  unsigned int reuseaddr;

  *fds_p = NULL;
  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  _DBUS_ZERO (hints);

  if (!family)
    hints.ai_family = AF_UNSPEC;
  else if (!strcmp(family, "ipv4"))
    hints.ai_family = AF_INET;
  else if (!strcmp(family, "ipv6"))
    hints.ai_family = AF_INET6;
  else
    {
      dbus_set_error (error,
                      DBUS_ERROR_BAD_ADDRESS,
                      "Unknown address family %s", family);
      return -1;
    }

  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;

 redo_lookup_with_port:
  ai = NULL;
  if ((res = getaddrinfo(host, port, &hints, &ai)) != 0 || !ai)
    {
      dbus_set_error (error,
                      _dbus_error_from_errno (errno),
                      "Failed to lookup host/port: \"%s:%s\": %s (%d)",
                      host ? host : "*", port, gai_strerror(res), res);
      goto failed;
    }

  tmp = ai;
  while (tmp)
    {
      int fd = -1, *newlisten_fd;
      if (!_dbus_open_socket (&fd, tmp->ai_family, SOCK_STREAM, 0, error))
        {
          _DBUS_ASSERT_ERROR_IS_SET(error);
          goto failed;
        }
      _DBUS_ASSERT_ERROR_IS_CLEAR(error);

      reuseaddr = 1;
      if (setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr))==-1)
        {
          _dbus_warn ("Failed to set socket option \"%s:%s\": %s",
                      host ? host : "*", port, _dbus_strerror (errno));
        }

      if (bind (fd, (struct sockaddr*) tmp->ai_addr, tmp->ai_addrlen) < 0)
        {
          saved_errno = errno;
          _dbus_close(fd, NULL);
          if (saved_errno == EADDRINUSE)
            {
              /* Depending on kernel policy, it may or may not
                 be neccessary to bind to both IPv4 & 6 addresses
                 so ignore EADDRINUSE here */
              tmp = tmp->ai_next;
              continue;
            }
          dbus_set_error (error, _dbus_error_from_errno (saved_errno),
                          "Failed to bind socket \"%s:%s\": %s",
                          host ? host : "*", port, _dbus_strerror (saved_errno));
          goto failed;
        }

      if (listen (fd, 30 /* backlog */) < 0)
        {
          saved_errno = errno;
          _dbus_close (fd, NULL);
          dbus_set_error (error, _dbus_error_from_errno (saved_errno),
                          "Failed to listen on socket \"%s:%s\": %s",
                          host ? host : "*", port, _dbus_strerror (saved_errno));
          goto failed;
        }

      newlisten_fd = dbus_realloc(listen_fd, sizeof(int)*(nlisten_fd+1));
      if (!newlisten_fd)
        {
          saved_errno = errno;
          _dbus_close (fd, NULL);
          dbus_set_error (error, _dbus_error_from_errno (saved_errno),
                          "Failed to allocate file handle array: %s",
                          _dbus_strerror (saved_errno));
          goto failed;
        }
      listen_fd = newlisten_fd;
      listen_fd[nlisten_fd] = fd;
      nlisten_fd++;

      if (!_dbus_string_get_length(retport))
        {
          /* If the user didn't specify a port, or used 0, then
             the kernel chooses a port. After the first address
             is bound to, we need to force all remaining addresses
             to use the same port */
          if (!port || !strcmp(port, "0"))
            {
              int result;
              struct sockaddr_storage addr;
              socklen_t addrlen;
              char portbuf[50];

              addrlen = sizeof(addr);
              result = getsockname(fd, (struct sockaddr*) &addr, &addrlen);

              if (result == -1 ||
                  (res = getnameinfo ((struct sockaddr*)&addr, addrlen, NULL, 0,
                                      portbuf, sizeof(portbuf),
                                      NI_NUMERICHOST)) != 0)
                {
                  dbus_set_error (error, _dbus_error_from_errno (errno),
                                  "Failed to resolve port \"%s:%s\": %s (%s)",
                                  host ? host : "*", port, gai_strerror(res), res);
                  goto failed;
                }
              if (!_dbus_string_append(retport, portbuf))
                {
                  dbus_set_error (error, DBUS_ERROR_NO_MEMORY, NULL);
                  goto failed;
                }

              /* Release current address list & redo lookup */
              port = _dbus_string_get_const_data(retport);
              freeaddrinfo(ai);
              goto redo_lookup_with_port;
            }
          else
            {
              if (!_dbus_string_append(retport, port))
                {
                    dbus_set_error (error, DBUS_ERROR_NO_MEMORY, NULL);
                    goto failed;
                }
            }
        }

      tmp = tmp->ai_next;
    }
  freeaddrinfo(ai);
  ai = NULL;

  if (!nlisten_fd)
    {
      errno = EADDRINUSE;
      dbus_set_error (error, _dbus_error_from_errno (errno),
                      "Failed to bind socket \"%s:%s\": %s",
                      host ? host : "*", port, _dbus_strerror (errno));
      goto failed;
    }

  for (i = 0 ; i < nlisten_fd ; i++)
    {
      if (!_dbus_set_fd_nonblocking (listen_fd[i], error))
        {
          goto failed;
        }
    }

  *fds_p = listen_fd;

  return nlisten_fd;

 failed:
  if (ai)
    freeaddrinfo(ai);
  for (i = 0 ; i < nlisten_fd ; i++)
    _dbus_close(listen_fd[i], NULL);
  dbus_free(listen_fd);
  return -1;
}
