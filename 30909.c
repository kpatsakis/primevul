user_update_system_account_property (User          *user,
                                     gboolean       system)
{
        accounts_user_set_system_account (ACCOUNTS_USER (user), system);
}
