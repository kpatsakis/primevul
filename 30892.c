user_new (Daemon *daemon,
          uid_t   uid)
{
        User *user;

        user = g_object_new (TYPE_USER, NULL);
        user->daemon = daemon;
        accounts_user_set_uid (ACCOUNTS_USER (user), uid);

        return user;
}
