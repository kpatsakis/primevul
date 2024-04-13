user_extension_get_value (User                    *user,
                          GDBusInterfaceInfo      *interface,
                          const GDBusPropertyInfo *property)
{
        const GVariantType *type = G_VARIANT_TYPE (property->signature);
        GVariant *value;
        g_autofree gchar *printed = NULL;
        gint i;

        /* First, try to get the value from the keyfile */
        printed = g_key_file_get_value (user->keyfile, interface->name, property->name, NULL);
        if (printed) {
                value = g_variant_parse (type, printed, NULL, NULL, NULL);
                if (value != NULL)
                        return value;
        }

        /* If that didn't work, try for a default value annotation */
        for (i = 0; property->annotations && property->annotations[i]; i++) {
                GDBusAnnotationInfo *annotation = property->annotations[i];

                if (g_str_equal (annotation->key, "org.freedesktop.Accounts.DefaultValue.String")) {
                        if (g_str_equal (property->signature, "s"))
                                return g_variant_ref_sink (g_variant_new_string (annotation->value));
                }
                else if (g_str_equal (annotation->key, "org.freedesktop.Accounts.DefaultValue")) {
                        value = g_variant_parse (type, annotation->value, NULL, NULL, NULL);
                        if (value != NULL)
                                return value;
                }
        }

        /* Nothing found... */
        return NULL;
}
