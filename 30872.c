user_change_email_authorized_cb (Daemon                *daemon,
                                 User                  *user,
                                 GDBusMethodInvocation *context,
                                 gpointer               data)

{
        gchar *email = data;

        if (g_strcmp0 (accounts_user_get_email (ACCOUNTS_USER (user)), email) != 0) {
                accounts_user_set_email (ACCOUNTS_USER (user), email);

                save_extra_data (user);
        }

        accounts_user_complete_set_email (ACCOUNTS_USER (user), context);
}
