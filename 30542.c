_dbus_connect_tcp_socket_with_nonce (const char     *host,
                                     const char     *port,
                                     const char     *family,
                                     const char     *noncefile,
                                     DBusError      *error)
{
  int fd = -1, res;
  struct addrinfo hints;
  struct addrinfo *ai, *tmp;

  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  _dbus_win_startup_winsock ();

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
                      DBUS_ERROR_INVALID_ARGS,
                      "Unknown address family %s", family);
      return -1;
    }
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_socktype = SOCK_STREAM;
#ifdef AI_ADDRCONFIG
  hints.ai_flags = AI_ADDRCONFIG;
#else
  hints.ai_flags = 0;
#endif

  if ((res = getaddrinfo(host, port, &hints, &ai)) != 0 || !ai)
    {
      dbus_set_error (error,
                      _dbus_error_from_errno (res),
                      "Failed to lookup host/port: \"%s:%s\": %s (%d)",
                      host, port, _dbus_strerror(res), res);
      return -1;
    }

  tmp = ai;
  while (tmp)
    {
      if ((fd = socket (tmp->ai_family, SOCK_STREAM, 0)) == INVALID_SOCKET)
        {
          DBUS_SOCKET_SET_ERRNO ();
          dbus_set_error (error,
                          _dbus_error_from_errno (errno),
                          "Failed to open socket: %s",
                          _dbus_strerror_from_errno ());
          freeaddrinfo(ai);
          return -1;
        }
      _DBUS_ASSERT_ERROR_IS_CLEAR(error);

      if (connect (fd, (struct sockaddr*) tmp->ai_addr, tmp->ai_addrlen) == SOCKET_ERROR)
        {
          DBUS_SOCKET_SET_ERRNO ();
          closesocket(fd);
          fd = -1;
          tmp = tmp->ai_next;
          continue;
        }

      break;
    }
  freeaddrinfo(ai);

  if (fd == -1)
    {
      dbus_set_error (error,
                      _dbus_error_from_errno (errno),
                      "Failed to connect to socket \"%s:%s\" %s",
                      host, port, _dbus_strerror_from_errno ());
      return -1;
    }

  if (noncefile != NULL)
    {
      DBusString noncefileStr;
      dbus_bool_t ret;
      if (!_dbus_string_init (&noncefileStr) ||
          !_dbus_string_append(&noncefileStr, noncefile))
        {
          closesocket (fd);
          dbus_set_error (error, DBUS_ERROR_NO_MEMORY, NULL);
          return -1;
        }

      ret = _dbus_send_nonce (fd, &noncefileStr, error);

      _dbus_string_free (&noncefileStr);

      if (!ret)
        {
          closesocket (fd);
          return -1;
        }
    }

  _dbus_fd_set_close_on_exec (fd);

  if (!_dbus_set_fd_nonblocking (fd, error))
    {
      closesocket (fd);
      return -1;
    }

  return fd;
}
