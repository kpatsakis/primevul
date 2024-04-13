cleanup (pam_handle_t *pamh UNUSED, void *data, int err UNUSED)
{
	free (data);
}
