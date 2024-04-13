static void dump_attrs(FILE *ofd, struct nlattr *attrs, int attrlen,
		       int prefix)
{
	int rem;
	struct nlattr *nla;

	nla_for_each_attr(nla, attrs, attrlen, rem) {
		int padlen, alen = nla_len(nla);

		prefix_line(ofd, prefix);

		if (nla->nla_type == 0)
			fprintf(ofd, "  [ATTR PADDING] %d octets\n", alen);
		else
			fprintf(ofd, "  [ATTR %02d%s] %d octets\n", nla_type(nla),
				nla_is_nested(nla) ? " NESTED" : "",
				alen);

		if (nla_is_nested(nla))
			dump_attrs(ofd, nla_data(nla), alen, prefix+1);
		else
			dump_attr(ofd, nla, prefix);

		padlen = nla_padlen(alen);
		if (padlen > 0) {
			prefix_line(ofd, prefix);
			fprintf(ofd, "  [PADDING] %d octets\n",
				padlen);
			dump_hex(ofd, nla_data(nla) + alen,
				 padlen, prefix);
		}
	}

	if (rem) {
		prefix_line(ofd, prefix);
		fprintf(ofd, "  [LEFTOVER] %d octets\n", rem);
	}
}
