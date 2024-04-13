user_set_real_name (AccountsUser          *auser,
                    GDBusMethodInvocation *context,
                    const gchar           *real_name)
{
        User *user = (User*)auser;
        int uid;
        const gchar *action_id;

        if (!get_caller_uid (context, &uid)) {
                throw_error (context, ERROR_FAILED, "identifying caller failed");
                return FALSE;
        }

        if (accounts_user_get_uid (ACCOUNTS_USER (user)) == (uid_t) uid)
                action_id = "org.freedesktop.accounts.change-own-user-data";
        else
                action_id = "org.freedesktop.accounts.user-administration";

        daemon_local_check_auth (user->daemon,
                                 user,
                                 action_id,
                                 TRUE,
                                 user_change_real_name_authorized_cb,
                                 context,
                                 g_strdup (real_name),
                                 (GDestroyNotify)g_free);

        return TRUE;
}
