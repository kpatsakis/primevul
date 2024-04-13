_dbus_win_set_errno (int err)
{
#ifdef DBUS_WINCE
  SetLastError (err);
#else
  errno = err;
#endif
}
