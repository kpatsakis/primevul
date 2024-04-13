limit_bandwidth (wgint bytes, struct ptimer *timer)
{
  double delta_t = ptimer_read (timer) - limit_data.chunk_start;
  double expected;

  limit_data.chunk_bytes += bytes;

  /* Calculate the amount of time we expect downloading the chunk
     should take.  If in reality it took less time, sleep to
     compensate for the difference.  */
  expected = (double) limit_data.chunk_bytes / opt.limit_rate;

  if (expected > delta_t)
    {
      double slp = expected - delta_t + limit_data.sleep_adjust;
      double t0, t1;
      if (slp < 0.2)
        {
          DEBUGP (("deferring a %.2f ms sleep (%s/%.2f).\n",
                   slp * 1000, number_to_static_string (limit_data.chunk_bytes),
                   delta_t));
          return;
        }
      DEBUGP (("\nsleeping %.2f ms for %s bytes, adjust %.2f ms\n",
               slp * 1000, number_to_static_string (limit_data.chunk_bytes),
               limit_data.sleep_adjust));

      t0 = ptimer_read (timer);
      xsleep (slp);
      t1 = ptimer_measure (timer);

      /* Due to scheduling, we probably slept slightly longer (or
         shorter) than desired.  Calculate the difference between the
         desired and the actual sleep, and adjust the next sleep by
         that amount.  */
      limit_data.sleep_adjust = slp - (t1 - t0);
      /* If sleep_adjust is very large, it's likely due to suspension
         and not clock inaccuracy.  Don't enforce those.  */
      if (limit_data.sleep_adjust > 0.5)
        limit_data.sleep_adjust = 0.5;
      else if (limit_data.sleep_adjust < -0.5)
        limit_data.sleep_adjust = -0.5;
    }

  limit_data.chunk_bytes = 0;
  limit_data.chunk_start = ptimer_read (timer);
}
