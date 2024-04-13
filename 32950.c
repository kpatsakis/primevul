user_local_register (User *user)
{
        GError *error = NULL;

        user->system_bus_connection = g_bus_get_sync (G_BUS_TYPE_SYSTEM, NULL, &error);
        if (user->system_bus_connection == NULL) {
                if (error != NULL) {
                        g_critical ("error getting system bus: %s", error->message);
                        g_error_free (error);
                }
                return;
        }

        user->object_path = compute_object_path (user);

        if (!g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (user),
                                               user->system_bus_connection,
                                               user->object_path,
                                               &error)) {
                if (error != NULL) {
                        g_critical ("error exporting user object: %s", error->message);
                        g_error_free (error);
                }
                return;
        }
}
