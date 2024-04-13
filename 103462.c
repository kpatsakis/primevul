gst_qtdemux_clip_buffer (GstQTDemux * qtdemux, QtDemuxStream * stream,
    GstBuffer * buf)
{
  gint64 start, stop, cstart, cstop, diff;
  GstClockTime timestamp = GST_CLOCK_TIME_NONE, duration = GST_CLOCK_TIME_NONE;
  guint8 *data;
  guint size;
  gint num_rate, denom_rate;
  gint frame_size;
  gboolean clip_data;

  data = GST_BUFFER_DATA (buf);
  size = GST_BUFFER_SIZE (buf);

  /* depending on the type, setup the clip parameters */
  if (stream->subtype == FOURCC_soun) {
    frame_size = stream->bytes_per_frame;
    num_rate = GST_SECOND;
    denom_rate = (gint) stream->rate;
    clip_data = TRUE;
  } else if (stream->subtype == FOURCC_vide) {
    frame_size = size;
    num_rate = stream->fps_n;
    denom_rate = stream->fps_d;
    clip_data = FALSE;
  } else
    goto wrong_type;

  /* we can only clip if we have a valid timestamp */
  timestamp = GST_BUFFER_TIMESTAMP (buf);
  if (!GST_CLOCK_TIME_IS_VALID (timestamp))
    goto no_timestamp;

  if (GST_BUFFER_DURATION_IS_VALID (buf)) {
    duration = GST_BUFFER_DURATION (buf);
  } else {
    duration =
        gst_util_uint64_scale_int (size / frame_size, num_rate, denom_rate);
  }

  start = timestamp;
  stop = start + duration;

  if (!gst_segment_clip (&stream->segment, GST_FORMAT_TIME,
          start, stop, &cstart, &cstop))
    goto clipped;

  /* see if some clipping happened */
  diff = cstart - start;
  if (diff > 0) {
    timestamp = cstart;
    duration -= diff;

    if (clip_data) {
      /* bring clipped time to samples and to bytes */
      diff = gst_util_uint64_scale_int (diff, denom_rate, num_rate);
      diff *= frame_size;

      GST_DEBUG_OBJECT (qtdemux, "clipping start to %" GST_TIME_FORMAT " %"
          G_GUINT64_FORMAT " bytes", GST_TIME_ARGS (cstart), diff);

      data += diff;
      size -= diff;
    }
  }
  diff = stop - cstop;
  if (diff > 0) {
    duration -= diff;

    if (clip_data) {
      /* bring clipped time to samples and then to bytes */
      diff = gst_util_uint64_scale_int (diff, denom_rate, num_rate);
      diff *= frame_size;

      GST_DEBUG_OBJECT (qtdemux, "clipping stop to %" GST_TIME_FORMAT " %"
          G_GUINT64_FORMAT " bytes", GST_TIME_ARGS (cstop), diff);

      size -= diff;
    }
  }

  GST_BUFFER_TIMESTAMP (buf) = timestamp;
  GST_BUFFER_DURATION (buf) = duration;
  GST_BUFFER_SIZE (buf) = size;
  GST_BUFFER_DATA (buf) = data;

  return buf;

  /* dropped buffer */
wrong_type:
  {
    GST_DEBUG_OBJECT (qtdemux, "unknown stream type");
    return buf;
  }
no_timestamp:
  {
    GST_DEBUG_OBJECT (qtdemux, "no timestamp on buffer");
    return buf;
  }
clipped:
  {
    GST_DEBUG_OBJECT (qtdemux, "clipped buffer");
    gst_buffer_unref (buf);
    return NULL;
  }
}
