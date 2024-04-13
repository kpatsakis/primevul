_dbus_get_monotonic_time (long *tv_sec,
                          long *tv_usec)
{
  /* no implementation yet, fall back to wall-clock time */
  _dbus_get_real_time (tv_sec, tv_usec);
}
