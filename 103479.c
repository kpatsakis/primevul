gst_qtdemux_loop_state_movie (GstQTDemux * qtdemux)
{
  GstFlowReturn ret = GST_FLOW_OK;
  GstBuffer *buf = NULL;
  QtDemuxStream *stream;
  guint64 min_time;
  guint64 offset;
  guint64 timestamp;
  guint64 duration;
  gboolean keyframe;
  guint size;
  gint index;
  gint i;

  gst_qtdemux_push_pending_newsegment (qtdemux);

  /* Figure out the next stream sample to output, min_time is expressed in
   * global time and runs over the edit list segments. */
  min_time = G_MAXUINT64;
  index = -1;
  for (i = 0; i < qtdemux->n_streams; i++) {
    guint64 position;

    stream = qtdemux->streams[i];
    position = stream->time_position;

    /* position of -1 is EOS */
    if (position != -1 && position < min_time) {
      min_time = position;
      index = i;
    }
  }
  /* all are EOS */
  if (index == -1) {
    GST_DEBUG_OBJECT (qtdemux, "all streams are EOS");
    goto eos;
  }

  /* check for segment end */
  if (qtdemux->segment.stop != -1 && qtdemux->segment.stop < min_time) {
    GST_DEBUG_OBJECT (qtdemux, "we reached the end of our segment.");
    goto eos;
  }

  stream = qtdemux->streams[index];

  /* fetch info for the current sample of this stream */
  if (!gst_qtdemux_prepare_current_sample (qtdemux, stream, &offset, &size,
          &timestamp, &duration, &keyframe))
    goto eos;

  GST_LOG_OBJECT (qtdemux,
      "pushing from stream %d, offset %" G_GUINT64_FORMAT
      ", size %d, timestamp=%" GST_TIME_FORMAT ", duration %" GST_TIME_FORMAT,
      index, offset, size, GST_TIME_ARGS (timestamp), GST_TIME_ARGS (duration));

  /* hmm, empty sample, skip and move to next sample */
  if (G_UNLIKELY (size <= 0))
    goto next;

  /* last pushed sample was out of boundary, goto next sample */
  if (stream->last_ret == GST_FLOW_UNEXPECTED)
    goto next;

  GST_LOG_OBJECT (qtdemux, "reading %d bytes @ %" G_GUINT64_FORMAT, size,
      offset);

  ret = gst_pad_pull_range (qtdemux->sinkpad, offset, size, &buf);
  if (ret != GST_FLOW_OK)
    goto beach;

  if (stream->fourcc == FOURCC_rtsp) {
    GstMessage *m;
    gchar *url;

    url = g_strndup ((gchar *) GST_BUFFER_DATA (buf), GST_BUFFER_SIZE (buf));

    /* we have RTSP redirect now */
    m = gst_message_new_element (GST_OBJECT_CAST (qtdemux),
        gst_structure_new ("redirect",
            "new-location", G_TYPE_STRING, url, NULL));
    g_free (url);

    gst_element_post_message (GST_ELEMENT_CAST (qtdemux), m);
  }

  qtdemux->last_ts = min_time;
  if (qtdemux->segment.rate >= 0) {
    gst_segment_set_last_stop (&qtdemux->segment, GST_FORMAT_TIME, min_time);
  }
  if (stream->pad) {
    /* we're going to modify the metadata */
    buf = gst_buffer_make_metadata_writable (buf);

    GST_BUFFER_TIMESTAMP (buf) = timestamp;
    GST_BUFFER_DURATION (buf) = duration;
    GST_BUFFER_OFFSET (buf) = -1;
    GST_BUFFER_OFFSET_END (buf) = -1;

    if (stream->need_clip)
      buf = gst_qtdemux_clip_buffer (qtdemux, stream, buf);

    if (buf == NULL)
      goto next;

    if (stream->discont) {
      GST_LOG_OBJECT (qtdemux, "marking discont buffer");
      GST_BUFFER_FLAG_SET (buf, GST_BUFFER_FLAG_DISCONT);
      stream->discont = FALSE;
    }

    if (!keyframe)
      GST_BUFFER_FLAG_SET (buf, GST_BUFFER_FLAG_DELTA_UNIT);

    gst_buffer_set_caps (buf, stream->caps);

    GST_LOG_OBJECT (qtdemux,
        "Pushing buffer with time %" GST_TIME_FORMAT ", duration %"
        GST_TIME_FORMAT " on pad %s",
        GST_TIME_ARGS (GST_BUFFER_TIMESTAMP (buf)),
        GST_TIME_ARGS (GST_BUFFER_DURATION (buf)), GST_PAD_NAME (stream->pad));

    ret = gst_pad_push (stream->pad, buf);
  } else {
    GST_DEBUG_OBJECT (qtdemux, "No output pad for stream, ignoring");
    gst_buffer_unref (buf);
    ret = GST_FLOW_OK;
  }

  /* combine flows */
  ret = gst_qtdemux_combine_flows (qtdemux, stream, ret);
  /* ignore unlinked, we will not push on the pad anymore and we will EOS when
   * we have no more data for the pad to push */
  if (ret == GST_FLOW_UNEXPECTED)
    ret = GST_FLOW_OK;

next:
  gst_qtdemux_advance_sample (qtdemux, stream);

beach:
  return ret;

  /* special cases */
eos:
  {
    GST_DEBUG_OBJECT (qtdemux, "No samples left for any streams - EOS");
    ret = GST_FLOW_UNEXPECTED;
    goto beach;
  }
}
