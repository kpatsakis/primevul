_dbus_daemon_unpublish_session_bus_address (void)
{
  HANDLE lock;

  lock = _dbus_global_lock( cUniqueDBusInitMutex );

  CloseHandle( hDBusSharedMem );

  hDBusSharedMem = NULL;

  ReleaseMutex( hDBusDaemonMutex );

  CloseHandle( hDBusDaemonMutex );

  hDBusDaemonMutex = NULL;

  _dbus_global_unlock( lock );
}
