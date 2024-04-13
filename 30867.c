compute_object_path (User *user)
{
        gchar *object_path;

        object_path = g_strdup_printf ("/org/freedesktop/Accounts/User%lu",
                                       (gulong) accounts_user_get_uid (ACCOUNTS_USER (user)));

        return object_path;
}
