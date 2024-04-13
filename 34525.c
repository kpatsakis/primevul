sd_source_finalize (GSource *source)
{
  SdSource *sd_source = (SdSource*)source;

  sd_login_monitor_unref (sd_source->monitor);
}
