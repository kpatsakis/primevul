gst_qtdemux_push_event (GstQTDemux * qtdemux, GstEvent * event)
{
  guint n;

  GST_DEBUG_OBJECT (qtdemux, "pushing %s event on all source pads",
      GST_EVENT_TYPE_NAME (event));

  for (n = 0; n < qtdemux->n_streams; n++) {
    GstPad *pad;

    if ((pad = qtdemux->streams[n]->pad))
      gst_pad_push_event (pad, gst_event_ref (event));
  }
  gst_event_unref (event);
}
