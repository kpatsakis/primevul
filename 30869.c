on_user_changed_timeout (User *user)
{
        user->changed_timeout_id = 0;
        accounts_user_emit_changed (ACCOUNTS_USER (user));

        return G_SOURCE_REMOVE;
}
