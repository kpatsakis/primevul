_dbus_get_config_file_name(DBusString *config_file, char *s)
{
  char path[MAX_PATH*2];
  int path_size = sizeof(path);

  if (!_dbus_get_install_root(path,path_size))
    return FALSE;

  if(strlen(s) + 4 + strlen(path) > sizeof(path)-2)
    return FALSE;
  strcat(path,"etc\\");
  strcat(path,s);
  if (_dbus_file_exists(path)) 
    {
      if (!_dbus_string_append (config_file, path))
        return FALSE;
    }
  else 
    {
      if (!_dbus_get_install_root(path,path_size))
        return FALSE;
      if(strlen(s) + 11 + strlen(path) > sizeof(path)-2)
        return FALSE;
      strcat(path,"etc\\dbus-1\\");
      strcat(path,s);
  
      if (_dbus_file_exists(path)) 
        {
          if (!_dbus_string_append (config_file, path))
            return FALSE;
        }
      else
        {
          if (!_dbus_get_install_root(path,path_size))
            return FALSE;
          if(strlen(s) + 4 + strlen(path) > sizeof(path)-2)
            return FALSE;
          strcat(path,"bus\\");
          strcat(path,s);
          
          if (_dbus_file_exists(path)) 
            {
              if (!_dbus_string_append (config_file, path))
                return FALSE;
            }
        }
    }
  return TRUE;
}    
