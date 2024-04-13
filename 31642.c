int parse_qvalue(const char *qvalue, const char **end)
{
	int q = 1000;

	if (!isdigit((unsigned char)*qvalue))
		goto out;
	q = (*qvalue++ - '0') * 1000;

	if (*qvalue++ != '.')
		goto out;

	if (!isdigit((unsigned char)*qvalue))
		goto out;
	q += (*qvalue++ - '0') * 100;

	if (!isdigit((unsigned char)*qvalue))
		goto out;
	q += (*qvalue++ - '0') * 10;

	if (!isdigit((unsigned char)*qvalue))
		goto out;
	q += (*qvalue++ - '0') * 1;
 out:
	if (q > 1000)
		q = 1000;
	if (end)
		*end = qvalue;
	return q;
}
