time_to_df(int h, int min, int s)
{
    return h * HOUR_IN_SECONDS + min * MINUTE_IN_SECONDS + s;
}