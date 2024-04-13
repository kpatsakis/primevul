test_ordinal(int from, int to, double sg)
{
    int j;

    fprintf(stderr, "test_ordinal: %d...%d (%d) - %.0f\n",
	    from, to, to - from, sg);
    for (j = from; j <= to; j++) {
	int y, d, rj, ns;

	c_jd_to_ordinal(j, sg, &y, &d);
	c_ordinal_to_jd(y, d, sg, &rj, &ns);
	if (j != rj) {
	    fprintf(stderr, "%d != %d\n", j, rj);
	    return 0;
	}
    }
    return 1;
}