static int compat_copy_entries(unsigned char *data, unsigned int size_user,
				struct ebt_entries_buf_state *state)
{
	unsigned int size_remaining = size_user;
	int ret;

	ret = EBT_ENTRY_ITERATE(data, size_user, size_entry_mwt, data,
					&size_remaining, state);
	if (ret < 0)
		return ret;

	WARN_ON(size_remaining);
	return state->buf_kern_offset;
}
