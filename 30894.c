user_register_extensions (User *user)
{
        static const GDBusInterfaceVTable vtable = {
                user_extension_method_call,
                NULL /* get_property */,
                NULL /* set_property */
        };
        GHashTable *extensions;
        GHashTableIter iter;
        gpointer iface;
        gint i = 0;

        g_assert (user->extension_ids == NULL);
        g_assert (user->n_extension_ids == 0);

        extensions = daemon_get_extension_ifaces (user->daemon);
        user->n_extension_ids = g_hash_table_size (extensions);
        user->extension_ids = g_new (guint, user->n_extension_ids);
        g_hash_table_iter_init (&iter, extensions);

        /* Ignore errors when registering more interfaces because (a)
         * they won't happen and (b) even if they do, we still want to
         * publish the main user interface.
         */
        while (g_hash_table_iter_next (&iter, NULL, &iface))
                user->extension_ids[i++] = g_dbus_connection_register_object (user->system_bus_connection,
                                                                              g_dbus_interface_skeleton_get_object_path (G_DBUS_INTERFACE_SKELETON (user)), iface,
                                                                              &vtable, user, NULL, NULL);
}
