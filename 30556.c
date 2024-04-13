_dbus_get_autolaunch_address (const char *scope, DBusString *address,
                              DBusError *error)
{
  HANDLE mutex;
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  dbus_bool_t retval = FALSE;
  LPSTR lpFile;
  char dbus_exe_path[MAX_PATH];
  char dbus_args[MAX_PATH * 2];
  const char * daemon_name = DBUS_DAEMON_NAME ".exe";
  DBusString shm_name;

  _DBUS_ASSERT_ERROR_IS_CLEAR (error);

  if (!_dbus_get_shm_name(&shm_name,scope))
    {
        dbus_set_error_const (error, DBUS_ERROR_FAILED, "could not determine shm name");
        return FALSE;
    }

  mutex = _dbus_global_lock ( cDBusAutolaunchMutex );

  if (_dbus_daemon_already_runs(address,&shm_name,scope))
    {
        _dbus_verbose( "found running dbus daemon at %s\n",
                       _dbus_string_get_const_data (&shm_name) );
        retval = TRUE;
        goto out;
    }

  if (!SearchPathA(NULL, daemon_name, NULL, sizeof(dbus_exe_path), dbus_exe_path, &lpFile))
    {
      HMODULE hmod;
      char dbus_module_path[MAX_PATH];
      DWORD rc;

      _dbus_verbose( "did not found dbus daemon executable on default search path, "
            "trying path where dbus shared library is located");

      hmod = _dbus_win_get_dll_hmodule();
      rc = GetModuleFileNameA(hmod, dbus_module_path, sizeof(dbus_module_path));
      if (rc <= 0)
        {
          dbus_set_error_const (error, DBUS_ERROR_FAILED, "could not retrieve dbus shared library file name");
          retval = FALSE;
          goto out;
        }
      else
        {
          char *ext_idx = strrchr(dbus_module_path, '\\');
          if (ext_idx)
          *ext_idx = '\0';
          if (!SearchPathA(dbus_module_path, daemon_name, NULL, sizeof(dbus_exe_path), dbus_exe_path, &lpFile))
            {
              dbus_set_error_const (error, DBUS_ERROR_FAILED, "could not find dbus-daemon executable");
              retval = FALSE;
              printf ("please add the path to %s to your PATH environment variable\n", daemon_name);
              printf ("or start the daemon manually\n\n");
              goto out;
            }
          _dbus_verbose( "found dbus daemon executable at %s",dbus_module_path);
        }
    }


  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  _snprintf(dbus_args, sizeof(dbus_args) - 1, "\"%s\" %s", dbus_exe_path,  " --session");

  if(CreateProcessA(dbus_exe_path, dbus_args, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
    {
      CloseHandle (pi.hThread);
      CloseHandle (pi.hProcess);
      retval = _dbus_get_autolaunch_shm( address, &shm_name );
      if (retval == FALSE)
        dbus_set_error_const (error, DBUS_ERROR_FAILED, "Failed to get autolaunch address from launched dbus-daemon");
    }
  else
    {
      dbus_set_error_const (error, DBUS_ERROR_FAILED, "Failed to launch dbus-daemon");
      retval = FALSE;
    }

out:
  if (retval)
    _DBUS_ASSERT_ERROR_IS_CLEAR (error);
  else
    _DBUS_ASSERT_ERROR_IS_SET (error);
  
  _dbus_global_unlock (mutex);

  return retval;
 }
