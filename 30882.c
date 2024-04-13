user_extension_set_property (User                  *user,
                             Daemon                *daemon,
                             GDBusInterfaceInfo    *interface,
                             GDBusMethodInvocation *invocation)
{
        const GDBusPropertyInfo *property = g_dbus_method_invocation_get_property_info (invocation);
        g_autoptr(GVariant) value = NULL;
        g_autofree gchar *printed = NULL;
        g_autofree gchar *prev = NULL;

        g_variant_get_child (g_dbus_method_invocation_get_parameters (invocation), 2, "v", &value);

        /* We'll always have the type when we parse it back so
         * we don't need it to be printed with annotations.
         */
        printed = g_variant_print (value, FALSE);

        /* May as well try to avoid the thrashing... */
        prev = g_key_file_get_value (user->keyfile, interface->name, property->name, NULL);

        if (!prev || !g_str_equal (printed, prev)) {
                g_key_file_set_value (user->keyfile, interface->name, property->name, printed);

                /* Emit a change signal.  Use invalidation
                 * because the data may not be world-readable.
                 */
                g_dbus_connection_emit_signal (g_dbus_method_invocation_get_connection (invocation),
                                               NULL, /* destination_bus_name */
                                               g_dbus_method_invocation_get_object_path (invocation),
                                               "org.freedesktop.DBus.Properties", "PropertiesChanged",
                                               g_variant_new_parsed ("( %s, %a{sv}, [ %s ] )",
                                                                     interface->name, NULL, property->name),
                                               NULL);

                accounts_user_emit_changed (ACCOUNTS_USER (user));
                save_extra_data (user);
        }

        g_dbus_method_invocation_return_value (invocation, g_variant_new ("()"));
}
