int nl_str2nlmsgtype(const char *name)
{
	return __str2type(name, nl_msgtypes, ARRAY_SIZE(nl_msgtypes));
}
