c_ordinal_to_jd(int y, int d, double sg, int *rjd, int *ns)
{
    int ns2;

    c_find_fdoy(y, sg, rjd, &ns2);
    *rjd += d - 1;
    *ns = (*rjd < sg) ? 0 : 1;
}