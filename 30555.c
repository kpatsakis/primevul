_dbus_get_address_string (DBusString *out, const char *basestring, const char *scope)
{
  _dbus_string_init(out);
  _dbus_string_append(out,basestring);

  if (!scope)
    {
      return TRUE;
    }
  else if (strcmp(scope,"*install-path") == 0
        || strcmp(scope,"install-path") == 0)
    {
      DBusString temp;
      if (!_dbus_get_install_root_as_hash(&temp))
        {
          _dbus_string_free(out);
           return FALSE;
        }
      _dbus_string_append(out,"-");
      _dbus_string_append(out,_dbus_string_get_const_data(&temp));
      _dbus_string_free(&temp);
    }
  else if (strcmp(scope,"*user") == 0)
    {
      _dbus_string_append(out,"-");
      if (!_dbus_append_user_from_current_process(out))
        {
           _dbus_string_free(out);
           return FALSE;
        }
    }
  else if (strlen(scope) > 0)
    {
      _dbus_string_append(out,"-");
      _dbus_string_append(out,scope);
      return TRUE;
    }
  return TRUE;
}
