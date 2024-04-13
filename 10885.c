test_nth_kday(int from, int to, double sg)
{
    int j;

    fprintf(stderr, "test_nth_kday: %d...%d (%d) - %.0f\n",
	    from, to, to - from, sg);
    for (j = from; j <= to; j++) {
	int y, m, n, k, rj, ns;

	c_jd_to_nth_kday(j, sg, &y, &m, &n, &k);
	c_nth_kday_to_jd(y, m, n, k, sg, &rj, &ns);
	if (j != rj) {
	    fprintf(stderr, "%d != %d\n", j, rj);
	    return 0;
	}
    }
    return 1;
}