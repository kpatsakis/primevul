user_local_update_from_keyfile (User     *user,
                                GKeyFile *keyfile)
{
        gchar *s;

        g_object_freeze_notify (G_OBJECT (user));

        s = g_key_file_get_string (keyfile, "User", "Language", NULL);
        if (s != NULL) {
                /* TODO: validate / normalize */
                g_free (user->language);
                user->language = s;
        }

        s = g_key_file_get_string (keyfile, "User", "XSession", NULL);
        if (s != NULL) {
                g_free (user->x_session);
                user->x_session = s;
        }

        s = g_key_file_get_string (keyfile, "User", "Email", NULL);
        if (s != NULL) {
                g_free (user->email);
                user->email = s;
        }

        s = g_key_file_get_string (keyfile, "User", "Location", NULL);
        if (s != NULL) {
                g_free (user->location);
                user->location = s;
        }

        s = g_key_file_get_string (keyfile, "User", "PasswordHint", NULL);
        if (s != NULL) {
                g_free (user->password_hint);
                user->password_hint = s;
        }

        s = g_key_file_get_string (keyfile, "User", "Icon", NULL);
        if (s != NULL) {
                g_free (user->icon_file);
                user->icon_file = s;
        }

        g_object_thaw_notify (G_OBJECT (user));
}
