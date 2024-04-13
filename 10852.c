c_valid_ordinal_p(int y, int d, double sg,
		  int *rd, int *rjd, int *ns)
{
    int ry2, rd2;

    if (d < 0) {
	int rjd2, ns2;

	if (!c_find_ldoy(y, sg, &rjd2, &ns2))
	    return 0;
	c_jd_to_ordinal(rjd2 + d + 1, sg, &ry2, &rd2);
	if (ry2 != y)
	    return 0;
	d = rd2;
    }
    c_ordinal_to_jd(y, d, sg, rjd, ns);
    c_jd_to_ordinal(*rjd, sg, &ry2, &rd2);
    if (ry2 != y || rd2 != d)
	return 0;
    return 1;
}