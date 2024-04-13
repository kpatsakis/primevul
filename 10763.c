c_valid_civil_p(int y, int m, int d, double sg,
		int *rm, int *rd, int *rjd, int *ns)
{
    int ry;

    if (m < 0)
	m += 13;
    if (d < 0) {
	if (!c_find_ldom(y, m, sg, rjd, ns))
	    return 0;
	c_jd_to_civil(*rjd + d + 1, sg, &ry, rm, rd);
	if (ry != y || *rm != m)
	    return 0;
	d = *rd;
    }
    c_civil_to_jd(y, m, d, sg, rjd, ns);
    c_jd_to_civil(*rjd, sg, &ry, rm, rd);
    if (ry != y || *rm != m || *rd != d)
	return 0;
    return 1;
}