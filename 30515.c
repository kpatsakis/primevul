_dbus_get_standard_system_servicedirs (DBusList **dirs)
{
  /*
   * DBUS_DATADIR may be the same as one of the standard directories. However,
   * the config parser should take care of the duplicates.
   *
   * Also, append /lib as counterpart of /usr/share on the root
   * directory (the root directory does not know /share), in order to
   * facilitate early boot system bus activation where /usr might not
   * be available.
   */
  static const char standard_search_path[] =
    "/usr/local/share:"
    "/usr/share:"
    DBUS_DATADIR ":"
    "/lib";
  DBusString servicedir_path;

  _dbus_string_init_const (&servicedir_path, standard_search_path);

  return _dbus_split_paths_and_append (&servicedir_path,
                                       DBUS_UNIX_STANDARD_SYSTEM_SERVICEDIR,
                                       dirs);
}
