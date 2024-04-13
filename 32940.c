compute_object_path (User *user)
{
        gchar *object_path;

        object_path = g_strdup_printf ("/org/freedesktop/Accounts/User%ld",
                                       (long) user->uid);

        return object_path;
}
