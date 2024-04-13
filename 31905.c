gmt_offset (time_t s)
{
  long gmtoff;

#if !HAVE_TM_GMTOFF
  struct tm tm_local = *localtime (&s);
  struct tm tm_gmt   = *gmtime (&s);

  gmtoff = tm_diff (&tm_local, &tm_gmt);
#else
  gmtoff = localtime (&s)->tm_gmtoff;
#endif

  return gmtoff;
}
