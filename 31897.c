change_env (timezone_t tz)
{
  if (setenv_TZ (tz->tz_is_set ? tz->abbrs : NULL) != 0)
    return false;
  tzset ();
  return true;
}
