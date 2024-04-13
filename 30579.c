_dbus_send_credentials_socket (int            handle,
                        DBusError      *error)
{
/* FIXME: for the session bus credentials shouldn't matter (?), but
 * for the system bus they are presumably essential. A rough outline
 * of a way to implement the credential transfer would be this:
 *
 * client waits to *read* a byte.
 *
 * server creates a named pipe with a random name, sends a byte
 * contining its length, and its name.
 *
 * client reads the name, connects to it (using Win32 API).
 *
 * server waits for connection to the named pipe, then calls
 * ImpersonateNamedPipeClient(), notes its now-current credentials,
 * calls RevertToSelf(), closes its handles to the named pipe, and
 * is done. (Maybe there is some other way to get the SID of a named
 * pipe client without having to use impersonation?)
 *
 * client closes its handles and is done.
 * 
 * Ralf: Why not sending credentials over the given this connection ?
 * Using named pipes makes it impossible to be connected from a unix client.  
 *
 */
  int bytes_written;
  DBusString buf; 

  _dbus_string_init_const_len (&buf, "\0", 1);
again:
  bytes_written = _dbus_write_socket (handle, &buf, 0, 1 );

  if (bytes_written < 0 && errno == EINTR)
    goto again;

  if (bytes_written < 0)
    {
      dbus_set_error (error, _dbus_error_from_errno (errno),
                      "Failed to write credentials byte: %s",
                     _dbus_strerror_from_errno ());
      return FALSE;
    }
  else if (bytes_written == 0)
    {
      dbus_set_error (error, DBUS_ERROR_IO_ERROR,
                      "wrote zero bytes writing credentials byte");
      return FALSE;
    }
  else
    {
      _dbus_assert (bytes_written == 1);
      _dbus_verbose ("wrote 1 zero byte, credential sending isn't implemented yet\n");
      return TRUE;
    }
  return TRUE;
}
