move_extra_data (const gchar *old_name,
                 const gchar *new_name)
{
        g_autofree gchar *old_filename = NULL;
        g_autofree gchar *new_filename = NULL;

        old_filename = g_build_filename (USERDIR,
                                         old_name, NULL);
        new_filename = g_build_filename (USERDIR,
                                         new_name, NULL);

        g_rename (old_filename, new_filename);
}
