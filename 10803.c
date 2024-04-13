c_jd_to_weeknum(int jd, int f, double sg, int *ry, int *rw, int *rd)
{
    int rm, rd2, rjd, ns, j;

    c_jd_to_civil(jd, sg, ry, &rm, &rd2);
    c_find_fdoy(*ry, sg, &rjd, &ns);
    rjd += 6;
    j = jd - (rjd - MOD((rjd - f) + 1, 7)) + 7;
    *rw = (int)DIV(j, 7);
    *rd = (int)MOD(j, 7);
}