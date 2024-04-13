_dbus_connect_tcp_socket_with_nonce (const char     *host,
                                     const char     *port,
                                     const char     *family,
                                     const char     *noncefile,
                                     DBusError      *error)
{
  int saved_errno = 0;
  int fd = -1, res;
  struct addrinfo hints;
  struct addrinfo *ai, *tmp;

  _DBUS_ASSERT_ERROR_IS_CLEAR(error);

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
  hints.ai_flags = AI_ADDRCONFIG;

  if ((res = getaddrinfo(host, port, &hints, &ai)) != 0)
    {
      dbus_set_error (error,
                      _dbus_error_from_errno (errno),
                      "Failed to lookup host/port: \"%s:%s\": %s (%d)",
                      host, port, gai_strerror(res), res);
      return -1;
    }

  tmp = ai;
  while (tmp)
    {
      if (!_dbus_open_socket (&fd, tmp->ai_family, SOCK_STREAM, 0, error))
        {
          freeaddrinfo(ai);
          _DBUS_ASSERT_ERROR_IS_SET(error);
          return -1;
        }
      _DBUS_ASSERT_ERROR_IS_CLEAR(error);

      if (connect (fd, (struct sockaddr*) tmp->ai_addr, tmp->ai_addrlen) < 0)
        {
          saved_errno = errno;
          _dbus_close(fd, NULL);
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
                      _dbus_error_from_errno (saved_errno),
                      "Failed to connect to socket \"%s:%s\" %s",
                      host, port, _dbus_strerror(saved_errno));
      return -1;
    }

  if (noncefile != NULL)
    {
      DBusString noncefileStr;
      dbus_bool_t ret;
      _dbus_string_init_const (&noncefileStr, noncefile);
      ret = _dbus_send_nonce (fd, &noncefileStr, error);
      _dbus_string_free (&noncefileStr);

      if (!ret)
    {
      _dbus_close (fd, NULL);
          return -1;
        }
    }

  if (!_dbus_set_fd_nonblocking (fd, error))
    {
      _dbus_close (fd, NULL);
      return -1;
    }

  return fd;
}
