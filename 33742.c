static void dump_attr(FILE *ofd, struct nlattr *attr, int prefix)
{
	int len = nla_len(attr);

	dump_hex(ofd, nla_data(attr), len, prefix);
}
