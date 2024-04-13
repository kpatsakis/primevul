qtdemux_parse_segments (GstQTDemux * qtdemux, QtDemuxStream * stream,
    GNode * trak)
{
  GNode *edts;

  /* parse and prepare segment info from the edit list */
  GST_DEBUG_OBJECT (qtdemux, "looking for edit list container");
  stream->n_segments = 0;
  stream->segments = NULL;
  if ((edts = qtdemux_tree_get_child_by_type (trak, FOURCC_edts))) {
    GNode *elst;
    gint n_segments;
    gint i, count;
    guint64 time, stime;
    guint8 *buffer;

    GST_DEBUG_OBJECT (qtdemux, "looking for edit list");
    if (!(elst = qtdemux_tree_get_child_by_type (edts, FOURCC_elst)))
      goto done;

    buffer = elst->data;

    n_segments = QT_UINT32 (buffer + 12);

    /* we might allocate a bit too much, at least allocate 1 segment */
    stream->segments = g_new (QtDemuxSegment, MAX (n_segments, 1));

    /* segments always start from 0 */
    time = 0;
    stime = 0;
    count = 0;
    for (i = 0; i < n_segments; i++) {
      guint64 duration;
      guint64 media_time;
      QtDemuxSegment *segment;
      guint32 rate_int;

      media_time = QT_UINT32 (buffer + 20 + i * 12);

      /* -1 media time is an empty segment, just ignore it */
      if (media_time == G_MAXUINT32)
        continue;

      duration = QT_UINT32 (buffer + 16 + i * 12);

      segment = &stream->segments[count++];

      /* time and duration expressed in global timescale */
      segment->time = stime;
      /* add non scaled values so we don't cause roundoff errors */
      time += duration;
      stime = gst_util_uint64_scale (time, GST_SECOND, qtdemux->timescale);
      segment->stop_time = stime;
      segment->duration = stime - segment->time;
      /* media_time expressed in stream timescale */
      segment->media_start =
          gst_util_uint64_scale (media_time, GST_SECOND, stream->timescale);
      segment->media_stop = segment->media_start + segment->duration;
      rate_int = GST_READ_UINT32_BE (buffer + 24 + i * 12);

      if (rate_int <= 1) {
        /* 0 is not allowed, some programs write 1 instead of the floating point
         * value */
        GST_WARNING_OBJECT (qtdemux, "found suspicious rate %" G_GUINT32_FORMAT,
            rate_int);
        segment->rate = 1;
      } else {
        segment->rate = rate_int / 65536.0;
      }

      GST_DEBUG_OBJECT (qtdemux, "created segment %d time %" GST_TIME_FORMAT
          ", duration %" GST_TIME_FORMAT ", media_time %" GST_TIME_FORMAT
          ", rate %g, (%d)", i, GST_TIME_ARGS (segment->time),
          GST_TIME_ARGS (segment->duration),
          GST_TIME_ARGS (segment->media_start), segment->rate, rate_int);
    }
    GST_DEBUG_OBJECT (qtdemux, "found %d non-empty segments", count);
    stream->n_segments = count;
  }
done:

  /* no segments, create one to play the complete trak */
  if (stream->n_segments == 0) {
    if (stream->segments == NULL)
      stream->segments = g_new (QtDemuxSegment, 1);

    stream->segments[0].time = 0;
    stream->segments[0].stop_time = qtdemux->segment.duration;
    stream->segments[0].duration = qtdemux->segment.duration;
    stream->segments[0].media_start = 0;
    stream->segments[0].media_stop = qtdemux->segment.duration;
    stream->segments[0].rate = 1.0;

    GST_DEBUG_OBJECT (qtdemux, "created dummy segment");
    stream->n_segments = 1;
  }
  GST_DEBUG_OBJECT (qtdemux, "using %d segments", stream->n_segments);

  return TRUE;
}
