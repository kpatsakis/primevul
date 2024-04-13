char *nl_nlmsgtype2str(int type, char *buf, size_t size)
{
	return __type2str(type, buf, size, nl_msgtypes,
			  ARRAY_SIZE(nl_msgtypes));
}
