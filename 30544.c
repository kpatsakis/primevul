_dbus_daemon_already_runs (DBusString *address, DBusString *shm_name, const char *scope)
{
  HANDLE lock;
  HANDLE daemon;
  DBusString mutex_name;
  dbus_bool_t bRet = TRUE;

  if (!_dbus_get_mutex_name(&mutex_name,scope))
    {
      _dbus_string_free( &mutex_name );
      return FALSE;
    }

  lock = _dbus_global_lock( cUniqueDBusInitMutex );

  daemon = CreateMutexA( NULL, FALSE, _dbus_string_get_const_data(&mutex_name) );
  if(WaitForSingleObject( daemon, 10 ) != WAIT_TIMEOUT)
    {
      ReleaseMutex (daemon);
      CloseHandle (daemon);

      _dbus_global_unlock( lock );
      _dbus_string_free( &mutex_name );
      return FALSE;
    }

  bRet = _dbus_get_autolaunch_shm( address, shm_name );

  CloseHandle ( daemon );

  _dbus_global_unlock( lock );
  _dbus_string_free( &mutex_name );

  return bRet;
}
