save_extra_data (User *user)
{
        gchar *filename;
        GKeyFile *keyfile;
        gchar *data;
        GError *error;

        keyfile = g_key_file_new ();
        user_local_save_to_keyfile (user, keyfile);

        error = NULL;
        data = g_key_file_to_data (keyfile, NULL, &error);
        if (error == NULL) {
                filename = g_build_filename ("/var/lib/AccountsService/users",
                                             user->user_name,
                                             NULL);
                g_file_set_contents (filename, data, -1, &error);
                g_free (filename);
        }
        if (error) {
                g_warning ("Saving data for user %s failed: %s",
                           user->user_name, error->message);
                g_error_free (error);
        }
        g_key_file_free (keyfile);
}
