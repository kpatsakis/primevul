_dbus_poll (DBusPollFD *fds,
            int         n_fds,
            int         timeout_milliseconds)
{
#define USE_CHRIS_IMPL 0

#if USE_CHRIS_IMPL

#define DBUS_POLL_CHAR_BUFFER_SIZE 2000
  char msg[DBUS_POLL_CHAR_BUFFER_SIZE];
  char *msgp;

  int ret = 0;
  int i;
  struct timeval tv;
  int ready;

#define DBUS_STACK_WSAEVENTS 256
  WSAEVENT eventsOnStack[DBUS_STACK_WSAEVENTS];
  WSAEVENT *pEvents = NULL;
  if (n_fds > DBUS_STACK_WSAEVENTS)
    pEvents = calloc(sizeof(WSAEVENT), n_fds);
  else
    pEvents = eventsOnStack;


#ifdef DBUS_ENABLE_VERBOSE_MODE
  msgp = msg;
  msgp += sprintf (msgp, "WSAEventSelect: to=%d\n\t", timeout_milliseconds);
  for (i = 0; i < n_fds; i++)
    {
      DBusPollFD *fdp = &fds[i];


      if (fdp->events & _DBUS_POLLIN)
        msgp += sprintf (msgp, "R:%d ", fdp->fd);

      if (fdp->events & _DBUS_POLLOUT)
        msgp += sprintf (msgp, "W:%d ", fdp->fd);

      msgp += sprintf (msgp, "E:%d\n\t", fdp->fd);

      if (msgp >= msg + DBUS_POLL_CHAR_BUFFER_SIZE)
        {
          _dbus_assert_not_reached ("buffer overflow in _dbus_poll");
        }
    }

  msgp += sprintf (msgp, "\n");
  _dbus_verbose ("%s",msg);
#endif
  for (i = 0; i < n_fds; i++)
    {
      DBusPollFD *fdp = &fds[i];
      WSAEVENT ev;
      long lNetworkEvents = FD_OOB;

      ev = WSACreateEvent();

      if (fdp->events & _DBUS_POLLIN)
        lNetworkEvents |= FD_READ | FD_ACCEPT | FD_CLOSE;

      if (fdp->events & _DBUS_POLLOUT)
        lNetworkEvents |= FD_WRITE | FD_CONNECT;

      WSAEventSelect(fdp->fd, ev, lNetworkEvents);

      pEvents[i] = ev;
    }


  ready = WSAWaitForMultipleEvents (n_fds, pEvents, FALSE, timeout_milliseconds, FALSE);

  if (DBUS_SOCKET_API_RETURNS_ERROR (ready))
    {
      DBUS_SOCKET_SET_ERRNO ();
      if (errno != WSAEWOULDBLOCK)
        _dbus_verbose ("WSAWaitForMultipleEvents: failed: %s\n", _dbus_strerror_from_errno ());
      ret = -1;
    }
  else if (ready == WSA_WAIT_TIMEOUT)
    {
      _dbus_verbose ("WSAWaitForMultipleEvents: WSA_WAIT_TIMEOUT\n");
      ret = 0;
    }
  else if (ready >= WSA_WAIT_EVENT_0 && ready < (int)(WSA_WAIT_EVENT_0 + n_fds))
    {
      msgp = msg;
      msgp += sprintf (msgp, "WSAWaitForMultipleEvents: =%d\n\t", ready);

      for (i = 0; i < n_fds; i++)
        {
          DBusPollFD *fdp = &fds[i];
          WSANETWORKEVENTS ne;

          fdp->revents = 0;

          WSAEnumNetworkEvents(fdp->fd, pEvents[i], &ne);

          if (ne.lNetworkEvents & (FD_READ | FD_ACCEPT | FD_CLOSE))
            fdp->revents |= _DBUS_POLLIN;

          if (ne.lNetworkEvents & (FD_WRITE | FD_CONNECT))
            fdp->revents |= _DBUS_POLLOUT;

          if (ne.lNetworkEvents & (FD_OOB))
            fdp->revents |= _DBUS_POLLERR;

          if (ne.lNetworkEvents & (FD_READ | FD_ACCEPT | FD_CLOSE))
              msgp += sprintf (msgp, "R:%d ", fdp->fd);

          if (ne.lNetworkEvents & (FD_WRITE | FD_CONNECT))
              msgp += sprintf (msgp, "W:%d ", fdp->fd);

          if (ne.lNetworkEvents & (FD_OOB))
              msgp += sprintf (msgp, "E:%d ", fdp->fd);

          msgp += sprintf (msgp, "lNetworkEvents:%d ", ne.lNetworkEvents);

          if(ne.lNetworkEvents)
            ret++;

          WSAEventSelect(fdp->fd, pEvents[i], 0);
        }

      msgp += sprintf (msgp, "\n");
      _dbus_verbose ("%s",msg);
    }
  else
    {
      _dbus_verbose ("WSAWaitForMultipleEvents: failed for unknown reason!");
      ret = -1;
    }

  for(i = 0; i < n_fds; i++)
    {
      WSACloseEvent(pEvents[i]);
    }

  if (n_fds > DBUS_STACK_WSAEVENTS)
    free(pEvents);

  return ret;

#else   /* USE_CHRIS_IMPL */

#define DBUS_POLL_CHAR_BUFFER_SIZE 2000
  char msg[DBUS_POLL_CHAR_BUFFER_SIZE];
  char *msgp;

  fd_set read_set, write_set, err_set;
  int max_fd = 0;
  int i;
  struct timeval tv;
  int ready;

  FD_ZERO (&read_set);
  FD_ZERO (&write_set);
  FD_ZERO (&err_set);


#ifdef DBUS_ENABLE_VERBOSE_MODE
  msgp = msg;
  msgp += sprintf (msgp, "select: to=%d\n\t", timeout_milliseconds);
  for (i = 0; i < n_fds; i++)
    {
      DBusPollFD *fdp = &fds[i];


      if (fdp->events & _DBUS_POLLIN)
        msgp += sprintf (msgp, "R:%d ", fdp->fd);

      if (fdp->events & _DBUS_POLLOUT)
        msgp += sprintf (msgp, "W:%d ", fdp->fd);

      msgp += sprintf (msgp, "E:%d\n\t", fdp->fd);

      if (msgp >= msg + DBUS_POLL_CHAR_BUFFER_SIZE)
        {
          _dbus_assert_not_reached ("buffer overflow in _dbus_poll");
        }
    }

  msgp += sprintf (msgp, "\n");
  _dbus_verbose ("%s",msg);
#endif
  for (i = 0; i < n_fds; i++)
    {
      DBusPollFD *fdp = &fds[i]; 

      if (fdp->events & _DBUS_POLLIN)
        FD_SET (fdp->fd, &read_set);

      if (fdp->events & _DBUS_POLLOUT)
        FD_SET (fdp->fd, &write_set);

      FD_SET (fdp->fd, &err_set);

      max_fd = MAX (max_fd, fdp->fd);
    }

  tv.tv_sec = timeout_milliseconds < 0 ? 1 : timeout_milliseconds / 1000;
  tv.tv_usec = timeout_milliseconds < 0 ? 0 : (timeout_milliseconds % 1000) * 1000;

  ready = select (max_fd + 1, &read_set, &write_set, &err_set, &tv);

  if (DBUS_SOCKET_API_RETURNS_ERROR (ready))
    {
      DBUS_SOCKET_SET_ERRNO ();
      if (errno != WSAEWOULDBLOCK)
        _dbus_verbose ("select: failed: %s\n", _dbus_strerror_from_errno ());
    }
  else if (ready == 0)
    _dbus_verbose ("select: = 0\n");
  else
    if (ready > 0)
      {
#ifdef DBUS_ENABLE_VERBOSE_MODE
        msgp = msg;
        msgp += sprintf (msgp, "select: = %d:\n\t", ready);

        for (i = 0; i < n_fds; i++)
          {
            DBusPollFD *fdp = &fds[i];

            if (FD_ISSET (fdp->fd, &read_set))
              msgp += sprintf (msgp, "R:%d ", fdp->fd);

            if (FD_ISSET (fdp->fd, &write_set))
              msgp += sprintf (msgp, "W:%d ", fdp->fd);

            if (FD_ISSET (fdp->fd, &err_set))
              msgp += sprintf (msgp, "E:%d\n\t", fdp->fd);
          }
        msgp += sprintf (msgp, "\n");
        _dbus_verbose ("%s",msg);
#endif

        for (i = 0; i < n_fds; i++)
          {
            DBusPollFD *fdp = &fds[i];

            fdp->revents = 0;

            if (FD_ISSET (fdp->fd, &read_set))
              fdp->revents |= _DBUS_POLLIN;

            if (FD_ISSET (fdp->fd, &write_set))
              fdp->revents |= _DBUS_POLLOUT;

            if (FD_ISSET (fdp->fd, &err_set))
              fdp->revents |= _DBUS_POLLERR;
          }
      }
  return ready;
#endif  /* USE_CHRIS_IMPL */
}
