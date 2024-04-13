user_change_language_authorized_cb (Daemon                *daemon,
                                    User                  *user,
                                    GDBusMethodInvocation *context,
                                    gpointer               data)

{
        gchar *language = data;

        if (g_strcmp0 (accounts_user_get_language (ACCOUNTS_USER (user)), language) != 0) {
                accounts_user_set_language (ACCOUNTS_USER (user), language);

                save_extra_data (user);
        }

        accounts_user_complete_set_language (ACCOUNTS_USER (user), context);
}
