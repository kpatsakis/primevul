int nlmsg_append(struct nl_msg *n, void *data, size_t len, int pad)
{
	void *tmp;

	tmp = nlmsg_reserve(n, len, pad);
	if (tmp == NULL)
		return -NLE_NOMEM;

	memcpy(tmp, data, len);
	NL_DBG(2, "msg %p: Appended %zu bytes with padding %d\n", n, len, pad);

	return 0;
}
