user_local_save_to_keyfile (User     *user,
                            GKeyFile *keyfile)
{
        if (user->email)
                g_key_file_set_string (keyfile, "User", "Email", user->email);

        if (user->language)
                g_key_file_set_string (keyfile, "User", "Language", user->language);

        if (user->x_session)
                g_key_file_set_string (keyfile, "User", "XSession", user->x_session);

        if (user->location)
                g_key_file_set_string (keyfile, "User", "Location", user->location);

        if (user->password_hint)
                g_key_file_set_string (keyfile, "User", "PasswordHint", user->password_hint);

        if (user->icon_file)
                g_key_file_set_string (keyfile, "User", "Icon", user->icon_file);
}
