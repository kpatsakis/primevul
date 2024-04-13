static void ima_log_string(struct audit_buffer *ab, char *key, char *value)
{
	audit_log_format(ab, "%s=", key);
	audit_log_untrustedstring(ab, value);
	audit_log_format(ab, " ");
}
