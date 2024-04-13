c_valid_weeknum_p(int y, int w, int d, int f, double sg,
		  int *rw, int *rd, int *rjd, int *ns)
{
    int ns2, ry2, rw2, rd2;

    if (d < 0)
	d += 7;
    if (w < 0) {
	int rjd2;

	c_weeknum_to_jd(y + 1, 1, f, f, sg, &rjd2, &ns2);
	c_jd_to_weeknum(rjd2 + w * 7, f, sg, &ry2, &rw2, &rd2);
	if (ry2 != y)
	    return 0;
	w = rw2;
    }
    c_weeknum_to_jd(y, w, d, f, sg, rjd, ns);
    c_jd_to_weeknum(*rjd, f, sg, &ry2, rw, rd);
    if (y != ry2 || w != *rw || d != *rd)
	return 0;
    return 1;
}