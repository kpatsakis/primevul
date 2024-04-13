localtime_rz (timezone_t tz, time_t const *t, struct tm *tm)
{
  if (!tz)
    return gmtime_r (t, tm);
  else
    {
      timezone_t old_tz = set_tz (tz);
      if (old_tz)
        {
          bool abbr_saved = localtime_r (t, tm) && save_abbr (tz, tm);
          if (revert_tz (old_tz) && abbr_saved)
            return tm;
        }
      return NULL;
    }
}
