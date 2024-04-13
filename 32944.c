user_change_real_name_authorized_cb (Daemon                *daemon,
                                     User                  *user,
                                     GDBusMethodInvocation *context,
                                     gpointer               data)

{
        gchar *name = data;
        GError *error;
        const gchar *argv[6];

        if (g_strcmp0 (user->real_name, name) != 0) {
                sys_log (context,
                         "change real name of user '%s' (%d) to '%s'",
                         user->user_name, user->uid, name);

                argv[0] = "/usr/sbin/usermod";
                argv[1] = "-c";
                argv[2] = name;
                argv[3] = "--";
                argv[4] = user->user_name;
                argv[5] = NULL;

                error = NULL;
                if (!spawn_with_login_uid (context, argv, &error)) {
                        throw_error (context, ERROR_FAILED, "running '%s' failed: %s", argv[0], error->message);
                        g_error_free (error);
                        return;
                }

                g_free (user->real_name);
                user->real_name = g_strdup (name);

                accounts_user_emit_changed (ACCOUNTS_USER (user));

                g_object_notify (G_OBJECT (user), "real-name");
        }

         accounts_user_complete_set_real_name (ACCOUNTS_USER (user), context);
 }
