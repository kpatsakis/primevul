c_valid_commercial_p(int y, int w, int d, double sg,
		     int *rw, int *rd, int *rjd, int *ns)
{
    int ns2, ry2, rw2, rd2;

    if (d < 0)
	d += 8;
    if (w < 0) {
	int rjd2;

	c_commercial_to_jd(y + 1, 1, 1, sg, &rjd2, &ns2);
	c_jd_to_commercial(rjd2 + w * 7, sg, &ry2, &rw2, &rd2);
	if (ry2 != y)
	    return 0;
	w = rw2;
    }
    c_commercial_to_jd(y, w, d, sg, rjd, ns);
    c_jd_to_commercial(*rjd, sg, &ry2, rw, rd);
    if (y != ry2 || w != *rw || d != *rd)
	return 0;
    return 1;
}