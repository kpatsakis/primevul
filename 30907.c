user_update_from_pwent (User          *user,
                        struct passwd *pwent,
                        struct spwd   *spent)
{
        g_autofree gchar *real_name = NULL;
        gboolean is_system_account;
        const gchar *passwd;
        gboolean locked;
        PasswordMode mode;
        AccountType account_type;

        g_object_freeze_notify (G_OBJECT (user));

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
                        g_clear_pointer (&real_name, g_free);
                }
        }
        else {
                real_name = NULL;
        }

        accounts_user_set_real_name (ACCOUNTS_USER (user), real_name);
        accounts_user_set_uid (ACCOUNTS_USER (user), pwent->pw_uid);

        user->gid = pwent->pw_gid;

        account_type = account_type_from_pwent (pwent);
        accounts_user_set_account_type (ACCOUNTS_USER (user), account_type);

        accounts_user_set_user_name (ACCOUNTS_USER (user), pwent->pw_name);
        accounts_user_set_home_directory (ACCOUNTS_USER (user), pwent->pw_dir);
        user_reset_icon_file (user);

        accounts_user_set_shell (ACCOUNTS_USER (user), pwent->pw_shell);

        passwd = NULL;
        if (spent)
                passwd = spent->sp_pwdp;

        if (passwd && passwd[0] == '!') {
                locked = TRUE;
        }
        else {
                locked = FALSE;
        }

        accounts_user_set_locked (ACCOUNTS_USER (user), locked);

        if (passwd == NULL || passwd[0] != 0) {
                mode = PASSWORD_MODE_REGULAR;
        }
        else {
                mode = PASSWORD_MODE_NONE;
        }

        if (spent) {
                if (spent->sp_lstchg == 0) {
                        mode = PASSWORD_MODE_SET_AT_LOGIN;
                }

                user->expiration_time = spent->sp_expire;
                user->last_change_time  = spent->sp_lstchg;
                user->min_days_between_changes = spent->sp_min;
                user->max_days_between_changes = spent->sp_max;
                user->days_to_warn  = spent->sp_warn;
                user->days_after_expiration_until_lock = spent->sp_inact;
                user->account_expiration_policy_known = TRUE;
        }

        accounts_user_set_password_mode (ACCOUNTS_USER (user), mode);
        is_system_account = !user_classify_is_human (accounts_user_get_uid (ACCOUNTS_USER (user)),
                                                     accounts_user_get_user_name (ACCOUNTS_USER (user)),
                                                     accounts_user_get_shell (ACCOUNTS_USER (user)),
                                                     passwd);
        accounts_user_set_system_account (ACCOUNTS_USER (user), is_system_account);

        g_object_thaw_notify (G_OBJECT (user));
}
