user_local_update_from_pwent (User          *user,
                              struct passwd *pwent)
{
#ifdef HAVE_SHADOW_H
        struct spwd *spent;
#endif
        gchar *real_name;
        gboolean changed;
        const gchar *passwd;
        gboolean locked;
        PasswordMode mode;

        g_object_freeze_notify (G_OBJECT (user));

        changed = FALSE;

        if (pwent->pw_gecos && pwent->pw_gecos[0] != '\0') {
                gchar *first_comma = NULL;
                gchar *valid_utf8_name = NULL;

                if (g_utf8_validate (pwent->pw_gecos, -1, NULL)) {
                        valid_utf8_name = pwent->pw_gecos;
                        first_comma = g_utf8_strchr (valid_utf8_name, -1, ',');
                }
                else {
                        g_warning ("User %s has invalid UTF-8 in GECOS field. "
                                   "It would be a good thing to check /etc/passwd.",
                                   pwent->pw_name ? pwent->pw_name : "");
                }

                if (first_comma) {
                        real_name = g_strndup (valid_utf8_name,
                                                  (first_comma - valid_utf8_name));
                }
                else if (valid_utf8_name) {
                        real_name = g_strdup (valid_utf8_name);
                }
                else {
                        real_name = NULL;
                }

                if (real_name && real_name[0] == '\0') {
                        g_free (real_name);
                        real_name = NULL;
                }
        }
        else {
                real_name = NULL;
        }
        if (g_strcmp0 (real_name, user->real_name) != 0) {
                g_free (user->real_name);
                user->real_name = real_name;
                changed = TRUE;
                g_object_notify (G_OBJECT (user), "real-name");
        }
        else {
                g_free (real_name);
        }

        /* UID */
        if (pwent->pw_uid != user->uid) {
                user->uid = pwent->pw_uid;
                changed = TRUE;
                g_object_notify (G_OBJECT (user), "uid");
        }

        /* GID */
        user->gid = pwent->pw_gid;

        user->account_type = account_type_from_pwent (pwent);

        /* Username */
        if (g_strcmp0 (user->user_name, pwent->pw_name) != 0) {
                g_free (user->user_name);
                user->user_name = g_strdup (pwent->pw_name);
                changed = TRUE;
                g_object_notify (G_OBJECT (user), "user-name");
        }

        /* Home Directory */
        if (g_strcmp0 (user->home_dir, pwent->pw_dir) != 0) {
                g_free (user->home_dir);
                user->home_dir = g_strdup (pwent->pw_dir);
                g_free (user->default_icon_file);
                user->default_icon_file = g_build_filename (user->home_dir, ".face", NULL);
                changed = TRUE;
                g_object_notify (G_OBJECT (user), "home-directory");
        }

        /* Shell */
        if (g_strcmp0 (user->shell, pwent->pw_shell) != 0) {
                g_free (user->shell);
                user->shell = g_strdup (pwent->pw_shell);
                changed = TRUE;
                g_object_notify (G_OBJECT (user), "shell");
        }

        passwd = pwent->pw_passwd;
#ifdef HAVE_SHADOW_H
        spent = getspnam (pwent->pw_name);
        if (spent)
                passwd = spent->sp_pwdp;
#endif

        if (passwd && passwd[0] == '!') {
                locked = TRUE;
        }
        else {
                locked = FALSE;
        }

        if (user->locked != locked) {
                user->locked = locked;
                changed = TRUE;
                g_object_notify (G_OBJECT (user), "locked");
        }

        if (passwd && passwd[0] != 0) {
                mode = PASSWORD_MODE_REGULAR;
        }
        else {
                mode = PASSWORD_MODE_NONE;
        }

#ifdef HAVE_SHADOW_H
        if (spent) {
                if (spent->sp_lstchg == 0) {
                        mode = PASSWORD_MODE_SET_AT_LOGIN;
                }
        }
#endif

        if (user->password_mode != mode) {
                user->password_mode = mode;
                changed = TRUE;
                g_object_notify (G_OBJECT (user), "password-mode");
        }

        user->system_account = daemon_local_user_is_excluded (user->daemon,
                                                              user->user_name,
                                                              pwent->pw_shell);

        g_object_thaw_notify (G_OBJECT (user));

        if (changed)
                accounts_user_emit_changed (ACCOUNTS_USER (user));
}
