user_local_unregister (User *user)
{
        g_dbus_interface_skeleton_unexport (G_DBUS_INTERFACE_SKELETON (user));
}
