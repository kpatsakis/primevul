c_julian_last_day_of_month(int y, int m)
{
    assert(m >= 1 && m <= 12);
    return monthtab[c_julian_leap_p(y) ? 1 : 0][m];
}