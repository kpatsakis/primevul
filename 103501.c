qtdemux_sink_activate_pull (GstPad * sinkpad, gboolean active)
{
  GstQTDemux *demux = GST_QTDEMUX (GST_PAD_PARENT (sinkpad));

  if (active) {
    demux->pullbased = TRUE;
    demux->segment_running = TRUE;
    return gst_pad_start_task (sinkpad, (GstTaskFunction) gst_qtdemux_loop,
        sinkpad);
  } else {
    demux->segment_running = FALSE;
    return gst_pad_stop_task (sinkpad);
  }
}
