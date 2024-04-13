c_find_fdoy(int y, double sg, int *rjd, int *ns)
{
    int d, rm, rd;

    for (d = 1; d < 31; d++)
	if (c_valid_civil_p(y, 1, d, sg, &rm, &rd, rjd, ns))
	    return 1;
    return 0;
}