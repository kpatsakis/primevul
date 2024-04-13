user_save_to_keyfile (User     *user,
                      GKeyFile *keyfile)
{
        g_key_file_remove_group (keyfile, "User", NULL);

        if (accounts_user_get_email (ACCOUNTS_USER (user)))
                g_key_file_set_string (keyfile, "User", "Email", accounts_user_get_email (ACCOUNTS_USER (user)));

        if (accounts_user_get_language (ACCOUNTS_USER (user)))
                g_key_file_set_string (keyfile, "User", "Language", accounts_user_get_language (ACCOUNTS_USER (user)));

        if (accounts_user_get_xsession (ACCOUNTS_USER (user)))
                g_key_file_set_string (keyfile, "User", "XSession", accounts_user_get_xsession (ACCOUNTS_USER (user)));

        if (accounts_user_get_location (ACCOUNTS_USER (user)))
                g_key_file_set_string (keyfile, "User", "Location", accounts_user_get_location (ACCOUNTS_USER (user)));

        if (accounts_user_get_password_hint (ACCOUNTS_USER (user)))
                g_key_file_set_string (keyfile, "User", "PasswordHint", accounts_user_get_password_hint (ACCOUNTS_USER (user)));

        if (accounts_user_get_icon_file (ACCOUNTS_USER (user)))
                g_key_file_set_string (keyfile, "User", "Icon", accounts_user_get_icon_file (ACCOUNTS_USER (user)));

        g_key_file_set_boolean (keyfile, "User", "SystemAccount", accounts_user_get_system_account (ACCOUNTS_USER (user)));

        user_set_cached (user, TRUE);
}
