gst_qtdemux_get_duration (GstQTDemux * qtdemux, gint64 * duration)
{
  gboolean res = TRUE;

  *duration = GST_CLOCK_TIME_NONE;

  if (qtdemux->duration != 0) {
    if (qtdemux->duration != G_MAXINT32 && qtdemux->timescale != 0) {
      *duration = gst_util_uint64_scale (qtdemux->duration,
          GST_SECOND, qtdemux->timescale);
    }
  }
  return res;
}
