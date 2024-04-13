user_extension_get_property (User                  *user,
                             Daemon                *daemon,
                             GDBusInterfaceInfo    *interface,
                             GDBusMethodInvocation *invocation)
{
        const GDBusPropertyInfo *property = g_dbus_method_invocation_get_property_info (invocation);
        g_autoptr(GVariant) value = NULL;

        value = user_extension_get_value (user, interface, property);

        if (value) {
                g_dbus_method_invocation_return_value (invocation, g_variant_new ("(v)", value));
        }
        else {
                g_dbus_method_invocation_return_error (invocation, G_DBUS_ERROR, G_DBUS_ERROR_INVALID_ARGS,
                                                       "Key '%s' is not set and has no default value",
                                                       property->name);
        }
}
