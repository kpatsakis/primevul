_dbus_get_autolaunch_shm (DBusString *address, DBusString *shm_name)
{
  HANDLE sharedMem;
  char *shared_addr;
  int i;

  for(i=0;i<20;++i) {
      sharedMem = OpenFileMappingA( FILE_MAP_READ, FALSE, _dbus_string_get_const_data(shm_name));
      if( sharedMem == 0 )
          Sleep( 100 );
      if ( sharedMem != 0)
          break;
  }

  if( sharedMem == 0 )
      return FALSE;

  shared_addr = MapViewOfFile( sharedMem, FILE_MAP_READ, 0, 0, 0 );

  if( !shared_addr )
      return FALSE;

  _dbus_string_init( address );

  _dbus_string_append( address, shared_addr );

  UnmapViewOfFile( shared_addr );

  CloseHandle( sharedMem );

  return TRUE;
}
