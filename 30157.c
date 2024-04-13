update_secret_string (NMSetting *setting,
                      const char *key,
                      const char *value,
                      GError **error)
{
	NMSettingVPNPrivate *priv = NM_SETTING_VPN_GET_PRIVATE (setting);

	g_return_val_if_fail (key != NULL, FALSE);
	g_return_val_if_fail (value != NULL, FALSE);

	if (!value || !strlen (value)) {
		g_set_error (error, NM_SETTING_ERROR,
		             NM_SETTING_ERROR_PROPERTY_TYPE_MISMATCH,
		             "Secret %s was empty", key);
		return FALSE;
	}

	g_hash_table_insert (priv->secrets, g_strdup (key), g_strdup (value));
	return TRUE;
}
