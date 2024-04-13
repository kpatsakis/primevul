user_change_user_name_authorized_cb (Daemon                *daemon,
                                     User                  *user,
                                     GDBusMethodInvocation *context,
                                     gpointer               data)

{
        gchar *name = data;
        gchar *old_name;
        g_autoptr(GError) error = NULL;
        const gchar *argv[6];

        if (g_strcmp0 (accounts_user_get_user_name (ACCOUNTS_USER (user)), name) != 0) {
                old_name = g_strdup (accounts_user_get_user_name (ACCOUNTS_USER (user)));
                sys_log (context,
                         "change name of user '%s' (%d) to '%s'",
                         old_name,
                         accounts_user_get_uid (ACCOUNTS_USER (user)),
                         name);

                argv[0] = "/usr/sbin/usermod";
                argv[1] = "-l";
                argv[2] = name;
                argv[3] = "--";
                argv[4] = accounts_user_get_user_name (ACCOUNTS_USER (user));
                argv[5] = NULL;

                if (!spawn_with_login_uid (context, argv, &error)) {
                        throw_error (context, ERROR_FAILED, "running '%s' failed: %s", argv[0], error->message);
                        return;
                }

                accounts_user_set_user_name (ACCOUNTS_USER (user), name);

                move_extra_data (old_name, name);
        }

        accounts_user_complete_set_user_name (ACCOUNTS_USER (user), context);
}
