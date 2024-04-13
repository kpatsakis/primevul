move_extra_data (const gchar *old_name,
                 const gchar *new_name)
{
        gchar *old_filename;
        gchar *new_filename;

        old_filename = g_build_filename ("/var/lib/AccountsService/users",
                                         old_name, NULL);
        new_filename = g_build_filename ("/var/lib/AccountsService/users",
                                         new_name, NULL);

        g_rename (old_filename, new_filename);

        g_free (old_filename);
        g_free (new_filename);
}
