sd_source_prepare (GSource *source,
                   gint    *timeout)
{
  *timeout = -1;
  return FALSE;
}
