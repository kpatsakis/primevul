HANDLE _dbus_global_lock (const char *mutexname)
{
  HANDLE mutex;
  DWORD gotMutex;

  mutex = CreateMutexA( NULL, FALSE, mutexname );
  if( !mutex )
    {
      return FALSE;
    }

   gotMutex = WaitForSingleObject( mutex, INFINITE );
   switch( gotMutex )
     {
       case WAIT_ABANDONED:
               ReleaseMutex (mutex);
               CloseHandle (mutex);
               return 0;
       case WAIT_FAILED:
       case WAIT_TIMEOUT:
               return 0;
     }

   return mutex;
}
