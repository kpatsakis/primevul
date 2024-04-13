user_change_location_authorized_cb (Daemon                *daemon,
                                    User                  *user,
                                    GDBusMethodInvocation *context,
                                    gpointer               data)

{
        gchar *location = data;

        if (g_strcmp0 (accounts_user_get_location (ACCOUNTS_USER (user)), location) != 0) {
                accounts_user_set_location (ACCOUNTS_USER (user), location);

                save_extra_data (user);
        }

        accounts_user_complete_set_location (ACCOUNTS_USER (user), context);
}
