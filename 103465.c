gst_qtdemux_do_seek (GstQTDemux * qtdemux, GstPad * pad, GstEvent * event)
{
  gdouble rate;
  GstFormat format;
  GstSeekFlags flags;
  GstSeekType cur_type, stop_type;
  gint64 cur, stop;
  gboolean flush;
  gboolean res;
  gboolean update;
  GstSegment seeksegment;
  int i;

  if (event) {
    GST_DEBUG_OBJECT (qtdemux, "doing seek with event");

    gst_event_parse_seek (event, &rate, &format, &flags,
        &cur_type, &cur, &stop_type, &stop);

    /* we have to have a format as the segment format. Try to convert
     * if not. */
    if (format != GST_FORMAT_TIME) {
      GstFormat fmt;

      fmt = GST_FORMAT_TIME;
      res = TRUE;
      if (cur_type != GST_SEEK_TYPE_NONE)
        res = gst_pad_query_convert (pad, format, cur, &fmt, &cur);
      if (res && stop_type != GST_SEEK_TYPE_NONE)
        res = gst_pad_query_convert (pad, format, stop, &fmt, &stop);
      if (!res)
        goto no_format;

      format = fmt;
    }
  } else {
    GST_DEBUG_OBJECT (qtdemux, "doing seek without event");
    flags = 0;
  }

  flush = flags & GST_SEEK_FLAG_FLUSH;

  GST_DEBUG_OBJECT (qtdemux, "seek format %d", format);

  /* stop streaming, either by flushing or by pausing the task */
  if (flush) {
    /* unlock upstream pull_range */
    gst_pad_push_event (qtdemux->sinkpad, gst_event_new_flush_start ());
    /* make sure out loop function exits */
    gst_qtdemux_push_event (qtdemux, gst_event_new_flush_start ());
  } else {
    /* non flushing seek, pause the task */
    gst_pad_pause_task (qtdemux->sinkpad);
  }

  /* wait for streaming to finish */
  GST_PAD_STREAM_LOCK (qtdemux->sinkpad);

  /* copy segment, we need this because we still need the old
   * segment when we close the current segment. */
  memcpy (&seeksegment, &qtdemux->segment, sizeof (GstSegment));

  if (event) {
    /* configure the segment with the seek variables */
    GST_DEBUG_OBJECT (qtdemux, "configuring seek");
    gst_segment_set_seek (&seeksegment, rate, format, flags,
        cur_type, cur, stop_type, stop, &update);
  }

  /* now do the seek, this actually never returns FALSE */
  res = gst_qtdemux_perform_seek (qtdemux, &seeksegment);

  /* prepare for streaming again */
  if (flush) {
    gst_pad_push_event (qtdemux->sinkpad, gst_event_new_flush_stop ());
    gst_qtdemux_push_event (qtdemux, gst_event_new_flush_stop ());
  } else if (qtdemux->segment_running) {
    /* we are running the current segment and doing a non-flushing seek,
     * close the segment first based on the last_stop. */
    GST_DEBUG_OBJECT (qtdemux, "closing running segment %" G_GINT64_FORMAT
        " to %" G_GINT64_FORMAT, qtdemux->segment.start,
        qtdemux->segment.last_stop);

    if (qtdemux->segment.rate >= 0) {
      /* FIXME, rate is the product of the global rate and the (quicktime)
       * segment rate. */
      qtdemux->pending_newsegment = gst_event_new_new_segment (TRUE,
          qtdemux->segment.rate, qtdemux->segment.format,
          qtdemux->segment.start, qtdemux->segment.last_stop,
          qtdemux->segment.time);
    } else {                    /* For Reverse Playback */
      guint64 stop;

      if ((stop = qtdemux->segment.stop) == -1)
        stop = qtdemux->segment.duration;
      /* for reverse playback, we played from stop to last_stop. */
      qtdemux->pending_newsegment = gst_event_new_new_segment (TRUE,
          qtdemux->segment.rate, qtdemux->segment.format,
          qtdemux->segment.last_stop, stop, qtdemux->segment.last_stop);
    }
  }

  /* commit the new segment */
  memcpy (&qtdemux->segment, &seeksegment, sizeof (GstSegment));

  if (qtdemux->segment.flags & GST_SEEK_FLAG_SEGMENT) {
    gst_element_post_message (GST_ELEMENT_CAST (qtdemux),
        gst_message_new_segment_start (GST_OBJECT_CAST (qtdemux),
            qtdemux->segment.format, qtdemux->segment.last_stop));
  }

  /* restart streaming, NEWSEGMENT will be sent from the streaming
   * thread. */
  qtdemux->segment_running = TRUE;
  for (i = 0; i < qtdemux->n_streams; i++)
    qtdemux->streams[i]->last_ret = GST_FLOW_OK;

  gst_pad_start_task (qtdemux->sinkpad, (GstTaskFunction) gst_qtdemux_loop,
      qtdemux->sinkpad);

  GST_PAD_STREAM_UNLOCK (qtdemux->sinkpad);

  return TRUE;

  /* ERRORS */
no_format:
  {
    GST_DEBUG_OBJECT (qtdemux, "unsupported format given, seek aborted.");
    return FALSE;
  }
}
