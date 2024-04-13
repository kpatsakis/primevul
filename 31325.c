get_owner_match_rule (const gchar *name)
{
      return g_strdup_printf ("type='signal',sender='" DBUS_SERVICE_DBUS
        "',path='" DBUS_PATH_DBUS
        "',interface='" DBUS_INTERFACE_DBUS
        "',member='NameOwnerChanged'"
        ",arg0='%s'", name);
}
