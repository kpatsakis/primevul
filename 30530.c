_dbus_append_keyring_directory_for_credentials (DBusString      *directory,
                                                DBusCredentials *credentials)
{
  DBusString homedir;
  DBusString dotdir;
  const char *homepath;
  const char *homedrive;

  _dbus_assert (credentials != NULL);
  _dbus_assert (!_dbus_credentials_are_anonymous (credentials));
  
  if (!_dbus_string_init (&homedir))
    return FALSE;

  homedrive = _dbus_getenv("HOMEDRIVE");
  if (homedrive != NULL && *homedrive != '\0')
    {
      _dbus_string_append(&homedir,homedrive);
    }

  homepath = _dbus_getenv("HOMEPATH");
  if (homepath != NULL && *homepath != '\0')
    {
      _dbus_string_append(&homedir,homepath);
    }
  
#ifdef DBUS_BUILD_TESTS
  {
    const char *override;
    
    override = _dbus_getenv ("DBUS_TEST_HOMEDIR");
    if (override != NULL && *override != '\0')
      {
        _dbus_string_set_length (&homedir, 0);
        if (!_dbus_string_append (&homedir, override))
          goto failed;

        _dbus_verbose ("Using fake homedir for testing: %s\n",
                       _dbus_string_get_const_data (&homedir));
      }
    else
      {
        static dbus_bool_t already_warned = FALSE;
        if (!already_warned)
          {
            _dbus_warn ("Using your real home directory for testing, set DBUS_TEST_HOMEDIR to avoid\n");
            already_warned = TRUE;
          }
      }
  }
#endif

#ifdef DBUS_WINCE
  /* It's not possible to create a .something directory in Windows CE
     using the file explorer.  */
#define KEYRING_DIR "dbus-keyrings"
#else
#define KEYRING_DIR ".dbus-keyrings"
#endif

  _dbus_string_init_const (&dotdir, KEYRING_DIR);
  if (!_dbus_concat_dir_and_file (&homedir,
                                  &dotdir))
    goto failed;
  
  if (!_dbus_string_copy (&homedir, 0,
                          directory, _dbus_string_get_length (directory))) {
    goto failed;
  }

  _dbus_string_free (&homedir);
  return TRUE;
  
 failed: 
  _dbus_string_free (&homedir);
  return FALSE;
}
