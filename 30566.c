_dbus_get_standard_session_servicedirs (DBusList **dirs)
{
  const char *common_progs;
  DBusString servicedir_path;

  if (!_dbus_string_init (&servicedir_path))
    return FALSE;

#ifdef DBUS_WINCE
  {
    /* On Windows CE, we adjust datadir dynamically to installation location.  */
    const char *data_dir = _dbus_getenv ("DBUS_DATADIR");

    if (data_dir != NULL)
      {
        if (!_dbus_string_append (&servicedir_path, data_dir))
          goto oom;
        
        if (!_dbus_string_append (&servicedir_path, _DBUS_PATH_SEPARATOR))
          goto oom;
      }
  }
#else
/*
 the code for accessing services requires absolute base pathes
 in case DBUS_DATADIR is relative make it absolute
*/
#ifdef DBUS_WIN
  {
    DBusString p;

    _dbus_string_init_const (&p, DBUS_DATADIR);

    if (!_dbus_path_is_absolute (&p))
      {
        char install_root[1000];
        if (_dbus_get_install_root (install_root, sizeof(install_root)))
          if (!_dbus_string_append (&servicedir_path, install_root))
            goto oom;
      }
  }
#endif
  if (!_dbus_string_append (&servicedir_path, DBUS_DATADIR))
    goto oom;

  if (!_dbus_string_append (&servicedir_path, _DBUS_PATH_SEPARATOR))
    goto oom;
#endif

  common_progs = _dbus_getenv ("CommonProgramFiles");

  if (common_progs != NULL)
    {
      if (!_dbus_string_append (&servicedir_path, common_progs))
        goto oom;

      if (!_dbus_string_append (&servicedir_path, _DBUS_PATH_SEPARATOR))
        goto oom;
    }

  if (!_dbus_split_paths_and_append (&servicedir_path, 
                               DBUS_STANDARD_SESSION_SERVICEDIR, 
                               dirs))
    goto oom;

  _dbus_string_free (&servicedir_path);  
  return TRUE;

 oom:
  _dbus_string_free (&servicedir_path);
  return FALSE;
}
