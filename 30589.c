_dbus_windows_get_datadir (void)
{
	return _dbus_replace_install_prefix(DBUS_DATADIR);
}
