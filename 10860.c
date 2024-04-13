c_jd_to_nth_kday(int jd, double sg, int *ry, int *rm, int *rn, int *rk)
{
    int rd, rjd, ns2;

    c_jd_to_civil(jd, sg, ry, rm, &rd);
    c_find_fdom(*ry, *rm, sg, &rjd, &ns2);
    *rn = DIV(jd - rjd, 7) + 1;
    *rk = c_jd_to_wday(jd);
}