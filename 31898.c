equal_tm (const struct tm *a, const struct tm *b)
{
  return ! ((a->tm_sec ^ b->tm_sec)
            | (a->tm_min ^ b->tm_min)
            | (a->tm_hour ^ b->tm_hour)
            | (a->tm_mday ^ b->tm_mday)
            | (a->tm_mon ^ b->tm_mon)
            | (a->tm_year ^ b->tm_year)
            | isdst_differ (a->tm_isdst, b->tm_isdst));
}
