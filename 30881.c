user_extension_method_call (GDBusConnection       *connection,
                            const gchar           *sender,
                            const gchar           *object_path,
                            const gchar           *interface_name,
                            const gchar           *method_name,
                            GVariant              *parameters,
                            GDBusMethodInvocation *invocation,
                            gpointer               user_data)
{
        User *user = user_data;
        GDBusInterfaceInfo *iface_info;
        const gchar *annotation_name;
        const gchar *action_id;
        gint uid;
        gint i;

        /* We don't allow method calls on extension interfaces, so we
         * should only ever see property calls here.
         */
        g_assert_cmpstr (interface_name, ==, "org.freedesktop.DBus.Properties");

        /* Now get the real interface name */
        g_variant_get_child (parameters, 0, "&s", &interface_name);

        if (get_caller_uid (invocation, &uid) && (uid_t) uid == accounts_user_get_uid (ACCOUNTS_USER (user))) {
                /* Operation on sender's own User object */
                if (g_str_equal (method_name, "Set")) {
                        annotation_name = "org.freedesktop.Accounts.Authentication.ChangeOwn";
                        action_id = "org.freedesktop.accounts.change-own-user-data";
                }
                else {
                        annotation_name = "org.freedesktop.Accounts.Authentication.ReadOwn";
                        action_id = ""; /* reading allowed by default */
                }
        }
        else {
                /* Operation on someone else's User object */
                if (g_str_equal (method_name, "Set")) {
                        annotation_name = "org.freedesktop.Accounts.Authentication.ChangeAny";
                        action_id = "org.freedesktop.accounts.user-administration";
                }
                else {
                        annotation_name = "org.freedesktop.Accounts.Authentication.ReadAny";
                        action_id = ""; /* reading allowed by default */
                }
        }

        iface_info = g_hash_table_lookup (daemon_get_extension_ifaces (user->daemon), interface_name);
        g_assert (iface_info != NULL);

        for (i = 0; iface_info->annotations && iface_info->annotations[i]; i++) {
                if (g_str_equal (iface_info->annotations[i]->key, annotation_name)) {
                        action_id = iface_info->annotations[i]->value;
                        break;
                }
        }

        if (action_id[0] == '\0') {
                /* Should always allow this call, so just do it now */
                user_extension_authentication_done (user->daemon, user, invocation, iface_info);
        }
        else {
                daemon_local_check_auth (user->daemon, user, action_id, TRUE,
                                         user_extension_authentication_done,
                                         invocation, iface_info, NULL);
        }
}
