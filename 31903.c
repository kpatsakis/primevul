mktime_z (timezone_t tz, struct tm *tm)
{
  if (!tz)
    return timegm (tm);
  else
    {
      timezone_t old_tz = set_tz (tz);
      if (old_tz)
        {
          time_t t = mktime (tm);
#if HAVE_TM_ZONE || HAVE_TZNAME
          time_t badtime = -1;
          struct tm tm_1;
          if ((t != badtime
               || (localtime_r (&t, &tm_1) && equal_tm (tm, &tm_1)))
              && !save_abbr (tz, tm))
            t = badtime;
#endif
          if (revert_tz (old_tz))
            return t;
        }
      return -1;
    }
}
