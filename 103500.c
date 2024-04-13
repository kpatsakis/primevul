qtdemux_sink_activate (GstPad * sinkpad)
{
  if (gst_pad_check_pull_range (sinkpad))
    return gst_pad_activate_pull (sinkpad, TRUE);
  else
    return gst_pad_activate_push (sinkpad, TRUE);
}
