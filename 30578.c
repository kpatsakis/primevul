_dbus_read_socket (int               fd,
                   DBusString       *buffer,
                   int               count)
{
  int bytes_read;
  int start;
  char *data;

  _dbus_assert (count >= 0);

  start = _dbus_string_get_length (buffer);

  if (!_dbus_string_lengthen (buffer, count))
    {
      _dbus_win_set_errno (ENOMEM);
      return -1;
    }

  data = _dbus_string_get_data_len (buffer, start, count);

 again:
 
  _dbus_verbose ("recv: count=%d fd=%d\n", count, fd);
  bytes_read = recv (fd, data, count, 0);
  
  if (bytes_read == SOCKET_ERROR)
	{
	  DBUS_SOCKET_SET_ERRNO();
	  _dbus_verbose ("recv: failed: %s (%d)\n", _dbus_strerror (errno), errno);
	  bytes_read = -1;
	}
	else
	  _dbus_verbose ("recv: = %d\n", bytes_read);

  if (bytes_read < 0)
    {
      if (errno == EINTR)
        goto again;
      else    	
        {
          /* put length back (note that this doesn't actually realloc anything) */
          _dbus_string_set_length (buffer, start);
          return -1;
        }
    }
  else
    {
      /* put length back (doesn't actually realloc) */
      _dbus_string_set_length (buffer, start + bytes_read);

#if 0
      if (bytes_read > 0)
        _dbus_verbose_bytes_of_string (buffer, start, bytes_read);
#endif

      return bytes_read;
    }
}
