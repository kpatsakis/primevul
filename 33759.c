int nlmsg_expand(struct nl_msg *n, size_t newlen)
{
	void *tmp;

	if (newlen <= n->nm_size)
		return -NLE_INVAL;

	tmp = realloc(n->nm_nlh, newlen);
	if (tmp == NULL)
		return -NLE_NOMEM;

	n->nm_nlh = tmp;
	n->nm_size = newlen;

	return 0;
}
