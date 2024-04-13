test_civil(int from, int to, double sg)
{
    int j;

    fprintf(stderr, "test_civil: %d...%d (%d) - %.0f\n",
	    from, to, to - from, sg);
    for (j = from; j <= to; j++) {
	int y, m, d, rj, ns;

	c_jd_to_civil(j, sg, &y, &m, &d);
	c_civil_to_jd(y, m, d, sg, &rj, &ns);
	if (j != rj) {
	    fprintf(stderr, "%d != %d\n", j, rj);
	    return 0;
	}
    }
    return 1;
}