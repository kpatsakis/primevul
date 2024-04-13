user_register (User *user)
{
        g_autoptr(GError) error = NULL;
        g_autofree gchar *object_path = NULL;

        user->system_bus_connection = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, &error);
        if (user->system_bus_connection == NULL) {
                if (error != NULL)
                        g_critical ("error getting system bus: %s", error->message);
                return;
        }

        object_path = compute_object_path (user);

        if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (user),
                                               user->system_bus_connection,
                                               object_path,
                                               &error)) {
                if (error != NULL)
                        g_critical ("error exporting user object: %s", error->message);
                return;
        }

        user_register_extensions (user);

        g_signal_connect (G_OBJECT (user), "notify", G_CALLBACK (on_user_property_notify), NULL);
}
