void _dbus_global_unlock (HANDLE mutex)
{
  ReleaseMutex (mutex);
  CloseHandle (mutex); 
}
