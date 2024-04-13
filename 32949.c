user_local_new (Daemon *daemon, uid_t uid)
{
        User *user;

        user = g_object_new (TYPE_USER, NULL);
        user->daemon = daemon;
        user->uid = uid;

        return user;
}
