on_user_property_notify (User *user)
{
        if (user->changed_timeout_id != 0)
                return;

        user->changed_timeout_id = g_timeout_add (250, (GSourceFunc) on_user_changed_timeout, user);
}
