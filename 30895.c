user_reset_icon_file (User *user)
{
        const char *icon_file;
        gboolean    icon_is_default;
        const char *home_dir;

        icon_file = accounts_user_get_icon_file (ACCOUNTS_USER (user));

        if (icon_file == NULL || g_strcmp0 (icon_file, user->default_icon_file) == 0) {
                icon_is_default = TRUE;
        } else {
                icon_is_default = FALSE;
        }

        g_free (user->default_icon_file);
        home_dir = accounts_user_get_home_directory (ACCOUNTS_USER (user));

        user->default_icon_file = g_build_filename (home_dir, ".face", NULL);

        if (icon_is_default) {
                accounts_user_set_icon_file (ACCOUNTS_USER (user), user->default_icon_file);
        }
}
