c_jd_to_ordinal(int jd, double sg, int *ry, int *rd)
{
    int rm2, rd2, rjd, ns;

    c_jd_to_civil(jd, sg, ry, &rm2, &rd2);
    c_find_fdoy(*ry, sg, &rjd, &ns);
    *rd = (jd - rjd) + 1;
}