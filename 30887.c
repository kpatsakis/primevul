user_get_password_expiration_policy_authorized_cb (Daemon                *daemon,
                                                   User                  *user,
                                                   GDBusMethodInvocation *context,
                                                   gpointer               data)

{
        if (!user->account_expiration_policy_known) {
                throw_error (context, ERROR_NOT_SUPPORTED, "account expiration policy unknown to accounts service");
                return;
        }

        accounts_user_complete_get_password_expiration_policy (ACCOUNTS_USER (user),
                                                               context,
                                                               user->expiration_time,
                                                               user->last_change_time,
                                                               user->min_days_between_changes,
                                                               user->max_days_between_changes,
                                                               user->days_to_warn,
                                                               user->days_after_expiration_until_lock);
}
