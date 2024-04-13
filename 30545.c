_dbus_daemon_is_session_bus_address_published (const char *scope)
{
  HANDLE lock;
  DBusString mutex_name;

  if (!_dbus_get_mutex_name(&mutex_name,scope))
    {
      _dbus_string_free( &mutex_name );
      return FALSE;
    }

  if (hDBusDaemonMutex)
      return TRUE;

  lock = _dbus_global_lock( cUniqueDBusInitMutex );

  hDBusDaemonMutex = CreateMutexA( NULL, FALSE, _dbus_string_get_const_data(&mutex_name) );

  /* The client uses mutex ownership to detect a running server, so the server should do so too.
     Fortunally the client deletes the mutex in the lock protected area, so checking presence 
     will work too.  */

  _dbus_global_unlock( lock );

  _dbus_string_free( &mutex_name );

  if (hDBusDaemonMutex  == NULL)
      return FALSE;
  if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
      CloseHandle(hDBusDaemonMutex);
      hDBusDaemonMutex = NULL;
      return TRUE;
    }
  return FALSE;
}
