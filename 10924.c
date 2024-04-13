c_jd_to_commercial(int jd, double sg, int *ry, int *rw, int *rd)
{
    int ry2, rm2, rd2, a, rjd2, ns2;

    c_jd_to_civil(jd - 3, sg, &ry2, &rm2, &rd2);
    a = ry2;
    c_commercial_to_jd(a + 1, 1, 1, sg, &rjd2, &ns2);
    if (jd >= rjd2)
	*ry = a + 1;
    else {
	c_commercial_to_jd(a, 1, 1, sg, &rjd2, &ns2);
	*ry = a;
    }
    *rw = 1 + DIV(jd - rjd2, 7);
    *rd = MOD(jd + 1, 7);
    if (*rd == 0)
	*rd = 7;
}