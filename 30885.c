user_get_object_path (User *user)
{
        return g_dbus_interface_skeleton_get_object_path (G_DBUS_INTERFACE_SKELETON (user));
}
