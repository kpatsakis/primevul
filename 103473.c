gst_qtdemux_handle_sink_event (GstPad * sinkpad, GstEvent * event)
{
  GstQTDemux *demux = GST_QTDEMUX (GST_PAD_PARENT (sinkpad));
  gboolean res;

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_NEWSEGMENT:
      /* We need to convert it to a GST_FORMAT_TIME new segment */
      gst_event_unref (event);
      res = TRUE;
      break;
    case GST_EVENT_EOS:
      /* If we are in push mode, and get an EOS before we've seen any streams,
       * then error out - we have nowhere to send the EOS */
      if (!demux->pullbased && demux->n_streams == 0) {
        GST_ELEMENT_ERROR (demux, STREAM, DECODE,
            (_("This file contains no playable streams.")),
            ("no known streams found"));
      }
      /* Fall through */
    default:
      res = gst_pad_event_default (demux->sinkpad, event);
      break;
  }

  return res;
}
