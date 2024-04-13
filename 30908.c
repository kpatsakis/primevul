user_update_local_account_property (User          *user,
                                    gboolean       local)
{
        accounts_user_set_local_account (ACCOUNTS_USER (user), local);
}
