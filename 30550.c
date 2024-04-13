_dbus_fd_set_close_on_exec (intptr_t handle)
{
  if ( !SetHandleInformation( (HANDLE) handle,
                        HANDLE_FLAG_INHERIT | HANDLE_FLAG_PROTECT_FROM_CLOSE,
                        0 /*disable both flags*/ ) )
    {
      _dbus_win_warn_win_error ("Disabling socket handle inheritance failed:", GetLastError());
    }
}
