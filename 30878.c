user_changed (User *user)
{
        accounts_user_emit_changed (ACCOUNTS_USER (user));
}
