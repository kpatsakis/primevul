get_secret_flags (NMSetting *setting,
                  const char *secret_name,
                  gboolean verify_secret,
                  NMSettingSecretFlags *out_flags,
                  GError **error)
{
	NMSettingVPNPrivate *priv = NM_SETTING_VPN_GET_PRIVATE (setting);
	gboolean success = FALSE;
	char *flags_key;
	gpointer val;
	unsigned long tmp;

	flags_key = g_strdup_printf ("%s-flags", secret_name);
	if (g_hash_table_lookup_extended (priv->data, flags_key, NULL, &val)) {
		errno = 0;
		tmp = strtoul ((const char *) val, NULL, 10);
		if ((errno == 0) && (tmp <= NM_SETTING_SECRET_FLAGS_ALL)) {
			if (out_flags)
				*out_flags = (guint32) tmp;
			success = TRUE;
		} else {
			g_set_error (error,
			             NM_SETTING_ERROR,
			             NM_SETTING_ERROR_PROPERTY_TYPE_MISMATCH,
			             "Failed to convert '%s' value '%s' to uint",
			             flags_key, (const char *) val);
		}
	} else {
		g_set_error (error,
		             NM_SETTING_ERROR,
		             NM_SETTING_ERROR_PROPERTY_NOT_FOUND,
		             "Secret flags property '%s' not found", flags_key);
	}
	g_free (flags_key);
	return success;
}
