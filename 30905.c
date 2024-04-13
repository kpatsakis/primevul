user_set_user_name (AccountsUser          *auser,
                    GDBusMethodInvocation *context,
                    const gchar           *user_name)
{
        User *user = (User*)auser;
        daemon_local_check_auth (user->daemon,
                                 user,
                                 "org.freedesktop.accounts.user-administration",
                                 TRUE,
                                 user_change_user_name_authorized_cb,
                                 context,
                                 g_strdup (user_name),
                                 (GDestroyNotify)g_free);

        return TRUE;
}
