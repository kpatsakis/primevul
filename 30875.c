user_change_shell_authorized_cb (Daemon                *daemon,
                                 User                  *user,
                                 GDBusMethodInvocation *context,
                                 gpointer               data)

{
        gchar *shell = data;
        g_autoptr(GError) error = NULL;
        const gchar *argv[6];

        if (g_strcmp0 (accounts_user_get_shell (ACCOUNTS_USER (user)), shell) != 0) {
                sys_log (context,
                         "change shell of user '%s' (%d) to '%s'",
                         accounts_user_get_user_name (ACCOUNTS_USER (user)),
                         accounts_user_get_uid (ACCOUNTS_USER (user)),
                         shell);

                argv[0] = "/usr/sbin/usermod";
                argv[1] = "-s";
                argv[2] = shell;
                argv[3] = "--";
                argv[4] = accounts_user_get_user_name (ACCOUNTS_USER (user));
                argv[5] = NULL;

                if (!spawn_with_login_uid (context, argv, &error)) {
                        throw_error (context, ERROR_FAILED, "running '%s' failed: %s", argv[0], error->message);
                        return;
                }

                accounts_user_set_shell (ACCOUNTS_USER (user), shell);
        }

        accounts_user_complete_set_shell (ACCOUNTS_USER (user), context);
}
