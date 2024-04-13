user_change_x_session_authorized_cb (Daemon                *daemon,
                                     User                  *user,
                                     GDBusMethodInvocation *context,
                                     gpointer               data)

{
        gchar *x_session = data;

        if (g_strcmp0 (accounts_user_get_xsession (ACCOUNTS_USER (user)), x_session) != 0) {
                accounts_user_set_xsession (ACCOUNTS_USER (user), x_session);

                save_extra_data (user);
        }

        accounts_user_complete_set_xsession (ACCOUNTS_USER (user), context);
}
