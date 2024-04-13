test_weeknum(int from, int to, int f, double sg)
{
    int j;

    fprintf(stderr, "test_weeknum: %d...%d (%d) - %.0f\n",
	    from, to, to - from, sg);
    for (j = from; j <= to; j++) {
	int y, w, d, rj, ns;

	c_jd_to_weeknum(j, f, sg, &y, &w, &d);
	c_weeknum_to_jd(y, w, d, f, sg, &rj, &ns);
	if (j != rj) {
	    fprintf(stderr, "%d != %d\n", j, rj);
	    return 0;
	}
    }
    return 1;
}