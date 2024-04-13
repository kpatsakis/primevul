irc_protocol_parse_time (const char *time)
{
    time_t time_value, time_msg, time_gm, time_local;
    struct tm tm_date, tm_date_gm, tm_date_local;
    long value;
    char *time2, *pos, *error;

    if (!time || !time[0])
        return 0;

    time_value = 0;

    if (strchr (time, '-'))
    {
        /* date is with ISO 8601 format: "2012-11-24T07:41:02.018Z" */
        /* initialize structure, because strptime does not do it */
        memset (&tm_date, 0, sizeof (struct tm));
        if (strptime (time, "%Y-%m-%dT%H:%M:%S", &tm_date))
        {
            if (tm_date.tm_year > 0)
            {
                time_msg = mktime (&tm_date);
                gmtime_r (&time_msg, &tm_date_gm);
                localtime_r (&time_msg, &tm_date_local);
                time_gm = mktime (&tm_date_gm);
                time_local = mktime (&tm_date_local);
                time_value = mktime (&tm_date_local) + (time_local - time_gm);
            }
        }
    }
    else
    {
        /* date is with timestamp format: "1353403519.478" */
        time2 = strdup (time);
        if (time2)
        {
            pos = strchr (time2, '.');
            if (pos)
                pos[0] = '\0';
            pos = strchr (time2, ',');
            if (pos)
                pos[0] = '\0';
            value = strtol (time2, &error, 10);
            if (error && !error[0] && (value >= 0))
                time_value = (int)value;
            free (time2);
        }
    }

    return time_value;
}