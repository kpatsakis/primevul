sd_source_check (GSource *source)
{
  SdSource *sd_source = (SdSource *)source;

  return sd_source->pollfd.revents != 0;
}
