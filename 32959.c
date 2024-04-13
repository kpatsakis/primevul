spawn_with_login_uid (GDBusMethodInvocation  *context,
                      const gchar            *argv[],
                      GError                **error)
{
        GError *local_error;
        gchar loginuid[20];
        gchar *std_err;
        gint status;

        get_caller_loginuid (context, loginuid, 20);

        local_error = NULL;
        std_err = NULL;

        if (!g_spawn_sync (NULL, (gchar**)argv, NULL, 0, setup_loginuid, loginuid, NULL, &std_err, &status, &local_error)) {
                g_propagate_error (error, local_error);
                g_free (std_err);
                return FALSE;
        }

        if (WEXITSTATUS (status) != 0) {
                g_set_error (error,
                             G_SPAWN_ERROR,
                             G_SPAWN_ERROR_FAILED,
                             "%s returned an error (%d): %s",
                             argv[0], WEXITSTATUS(status), std_err);
                g_free (std_err);
                return FALSE;
        }

        g_free (std_err);

        return TRUE;
}
