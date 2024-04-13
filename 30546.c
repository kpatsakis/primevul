_dbus_daemon_publish_session_bus_address (const char* address, const char *scope)
{
  HANDLE lock;
  char *shared_addr = NULL;
  DBusString shm_name;
  DBusString mutex_name;

  _dbus_assert (address);

  if (!_dbus_get_mutex_name(&mutex_name,scope))
    {
      _dbus_string_free( &mutex_name );
      return FALSE;
    }

  lock = _dbus_global_lock( cUniqueDBusInitMutex );

  if (!hDBusDaemonMutex)
    {
      hDBusDaemonMutex = CreateMutexA( NULL, FALSE, _dbus_string_get_const_data(&mutex_name) );
    }
  _dbus_string_free( &mutex_name );

  if (WaitForSingleObject( hDBusDaemonMutex, 10 ) != WAIT_OBJECT_0)
    {
      _dbus_global_unlock( lock );
      CloseHandle( hDBusDaemonMutex );
      return FALSE;
    }

  if (!_dbus_get_shm_name(&shm_name,scope))
    {
      _dbus_string_free( &shm_name );
      _dbus_global_unlock( lock );
      return FALSE;
    }

  hDBusSharedMem = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                                       0, strlen( address ) + 1, _dbus_string_get_const_data(&shm_name) );
  _dbus_assert( hDBusSharedMem );

  shared_addr = MapViewOfFile( hDBusSharedMem, FILE_MAP_WRITE, 0, 0, 0 );

  _dbus_assert (shared_addr);

  strcpy( shared_addr, address);

  UnmapViewOfFile( shared_addr );

  _dbus_global_unlock( lock );
  _dbus_verbose( "published session bus address at %s\n",_dbus_string_get_const_data (&shm_name) );

  _dbus_string_free( &shm_name );
  return TRUE;
}
