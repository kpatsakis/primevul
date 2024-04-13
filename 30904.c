user_set_shell (AccountsUser          *auser,
                GDBusMethodInvocation *context,
                const gchar           *shell)
{
        User *user = (User*)auser;
        daemon_local_check_auth (user->daemon,
                                 user,
                                 "org.freedesktop.accounts.user-administration",
                                 TRUE,
                                 user_change_shell_authorized_cb,
                                 context,
                                 g_strdup (shell),
                                 (GDestroyNotify)g_free);

        return TRUE;
}
