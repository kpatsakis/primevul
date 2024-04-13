c_nth_kday_to_jd(int y, int m, int n, int k, double sg, int *rjd, int *ns)
{
    int rjd2, ns2;

    if (n > 0) {
	c_find_fdom(y, m, sg, &rjd2, &ns2);
	rjd2 -= 1;
    }
    else {
	c_find_ldom(y, m, sg, &rjd2, &ns2);
	rjd2 += 7;
    }
    *rjd = (rjd2 - MOD((rjd2 - k) + 1, 7)) + 7 * n;
    *ns = (*rjd < sg) ? 0 : 1;
}