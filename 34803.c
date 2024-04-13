_tcp_tl_check_connected (struct eXosip_t *excontext)
{
  struct eXtltcp *reserved = (struct eXtltcp *) excontext->eXtltcp_reserved;
  int pos;
  int res;

  for (pos = 0; pos < EXOSIP_MAX_SOCKETS; pos++) {
    if (reserved->socket_tab[pos].invalid > 0) {
      OSIP_TRACE (osip_trace
                  (__FILE__, __LINE__, OSIP_INFO2, NULL,
                   "_tcp_tl_check_connected: socket node is in invalid state:%s:%i, socket %d [pos=%d], family:%d\n",
                   reserved->socket_tab[pos].remote_ip, reserved->socket_tab[pos].remote_port, reserved->socket_tab[pos].socket, pos, reserved->socket_tab[pos].ai_addr.sa_family));
      _tcp_tl_close_sockinfo (&reserved->socket_tab[pos]);
      continue;
    }

    if (reserved->socket_tab[pos].socket > 0 && reserved->socket_tab[pos].ai_addrlen > 0) {
      res = _tcp_tl_is_connected (reserved->socket_tab[pos].socket);
      if (res > 0) {
        res = connect (reserved->socket_tab[pos].socket, &reserved->socket_tab[pos].ai_addr, reserved->socket_tab[pos].ai_addrlen);
        OSIP_TRACE (osip_trace
                    (__FILE__, __LINE__, OSIP_INFO2, NULL,
                     "_tcp_tl_check_connected: socket node:%s:%i, socket %d [pos=%d], family:%d, in progress\n",
                     reserved->socket_tab[pos].remote_ip, reserved->socket_tab[pos].remote_port, reserved->socket_tab[pos].socket, pos, reserved->socket_tab[pos].ai_addr.sa_family));
        continue;
      }
      else if (res == 0) {
        OSIP_TRACE (osip_trace
                    (__FILE__, __LINE__, OSIP_INFO1, NULL,
                     "_tcp_tl_check_connected: socket node:%s:%i , socket %d [pos=%d], family:%d, connected\n",
                     reserved->socket_tab[pos].remote_ip, reserved->socket_tab[pos].remote_port, reserved->socket_tab[pos].socket, pos, reserved->socket_tab[pos].ai_addr.sa_family));
        /* stop calling "connect()" */
        reserved->socket_tab[pos].ai_addrlen = 0;
        continue;
      }
      else {
        OSIP_TRACE (osip_trace
                    (__FILE__, __LINE__, OSIP_INFO2, NULL,
                     "_tcp_tl_check_connected: socket node:%s:%i, socket %d [pos=%d], family:%d, error\n",
                     reserved->socket_tab[pos].remote_ip, reserved->socket_tab[pos].remote_port, reserved->socket_tab[pos].socket, pos, reserved->socket_tab[pos].ai_addr.sa_family));
        _tcp_tl_close_sockinfo (&reserved->socket_tab[pos]);
        continue;
      }
    }
  }
  return 0;
}
