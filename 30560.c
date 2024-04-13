_dbus_get_install_root_as_hash(DBusString *out)
{
    DBusString install_path;

    char path[MAX_PATH*2];
    int path_size = sizeof(path);

    if (!_dbus_get_install_root(path,path_size))
        return FALSE;

    _dbus_string_init(&install_path);
    _dbus_string_append(&install_path,path);

    _dbus_string_init(out);
    _dbus_string_tolower_ascii(&install_path,0,_dbus_string_get_length(&install_path));

    if (!_dbus_sha_compute (&install_path, out))
        return FALSE;

    return TRUE;
}
