gst_qtdemux_find_index (GstQTDemux * qtdemux, QtDemuxStream * str,
    guint64 media_time)
{
  guint32 i;

  if (str->n_samples == 0)
    return 0;

  for (i = 0; i < str->n_samples; i++) {
    if (str->samples[i].timestamp > media_time) {
      /* first sample after media_time, we need the previous one */
      return (i == 0 ? 0 : i - 1);
    }
  }
  return str->n_samples - 1;
}
