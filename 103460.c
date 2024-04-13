gst_qtdemux_change_state (GstElement * element, GstStateChange transition)
{
  GstQTDemux *qtdemux = GST_QTDEMUX (element);
  GstStateChangeReturn result = GST_STATE_CHANGE_FAILURE;

  switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      break;
    default:
      break;
  }

  result = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

  switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:{
      gint n;

      qtdemux->state = QTDEMUX_STATE_INITIAL;
      qtdemux->last_ts = GST_CLOCK_TIME_NONE;
      qtdemux->neededbytes = 16;
      qtdemux->todrop = 0;
      qtdemux->pullbased = FALSE;
      qtdemux->offset = 0;
      qtdemux->mdatoffset = GST_CLOCK_TIME_NONE;
      if (qtdemux->mdatbuffer)
        gst_buffer_unref (qtdemux->mdatbuffer);
      qtdemux->mdatbuffer = NULL;
      gst_adapter_clear (qtdemux->adapter);
      for (n = 0; n < qtdemux->n_streams; n++) {
        QtDemuxStream *stream = qtdemux->streams[n];

        while (stream->buffers) {
          gst_buffer_unref (GST_BUFFER_CAST (stream->buffers->data));
          stream->buffers =
              g_slist_delete_link (stream->buffers, stream->buffers);
        }
        if (stream->pad)
          gst_element_remove_pad (element, stream->pad);
        if (stream->samples)
          g_free (stream->samples);
        if (stream->caps)
          gst_caps_unref (stream->caps);
        if (stream->segments)
          g_free (stream->segments);
        g_free (stream);
      }
      qtdemux->major_brand = 0;
      qtdemux->n_streams = 0;
      qtdemux->n_video_streams = 0;
      qtdemux->n_audio_streams = 0;
      gst_segment_init (&qtdemux->segment, GST_FORMAT_TIME);
      break;
    }
    default:
      break;
  }

  return result;
}
