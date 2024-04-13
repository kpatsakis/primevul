test_commercial(int from, int to, double sg)
{
    int j;

    fprintf(stderr, "test_commercial: %d...%d (%d) - %.0f\n",
	    from, to, to - from, sg);
    for (j = from; j <= to; j++) {
	int y, w, d, rj, ns;

	c_jd_to_commercial(j, sg, &y, &w, &d);
	c_commercial_to_jd(y, w, d, sg, &rj, &ns);
	if (j != rj) {
	    fprintf(stderr, "%d != %d\n", j, rj);
	    return 0;
	}
    }
    return 1;
}