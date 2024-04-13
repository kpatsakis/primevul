gst_qtdemux_activate_segment (GstQTDemux * qtdemux, QtDemuxStream * stream,
    guint32 seg_idx, guint64 offset)
{
  GstEvent *event;
  QtDemuxSegment *segment;
  guint32 index, kf_index;
  guint64 seg_time;
  guint64 start, stop, time;
  gdouble rate;

  GST_LOG_OBJECT (qtdemux, "activate segment %d, offset %" G_GUINT64_FORMAT,
      seg_idx, offset);

  /* update the current segment */
  stream->segment_index = seg_idx;

  /* get the segment */
  segment = &stream->segments[seg_idx];

  if (offset < segment->time) {
    GST_WARNING_OBJECT (qtdemux, "offset < segment->time %" G_GUINT64_FORMAT,
        segment->time);
    return FALSE;
  }

  /* get time in this segment */
  seg_time = offset - segment->time;

  GST_LOG_OBJECT (qtdemux, "seg_time %" GST_TIME_FORMAT,
      GST_TIME_ARGS (seg_time));

  if (seg_time > segment->duration) {
    GST_LOG_OBJECT (qtdemux, "seg_time > segment->duration %" GST_TIME_FORMAT,
        GST_TIME_ARGS (segment->duration));
    return FALSE;
  }

  /* qtdemux->segment.stop is in outside-time-realm, whereas
   * segment->media_stop is in track-time-realm.
   * 
   * In order to compare the two, we need to bring segment.stop
   * into the track-time-realm */

  if (qtdemux->segment.stop == -1)
    stop = segment->media_stop;
  else
    stop =
        MIN (segment->media_stop,
        qtdemux->segment.stop - segment->time + segment->media_start);

  if (qtdemux->segment.rate >= 0) {
    start = MIN (segment->media_start + seg_time, stop);
    time = offset;
  } else {
    start = segment->media_start;
    stop = MIN (segment->media_start + seg_time, stop);
    time = segment->time;
  }

  GST_DEBUG_OBJECT (qtdemux, "newsegment %d from %" GST_TIME_FORMAT
      " to %" GST_TIME_FORMAT ", time %" GST_TIME_FORMAT, seg_idx,
      GST_TIME_ARGS (start), GST_TIME_ARGS (stop), GST_TIME_ARGS (time));

  /* combine global rate with that of the segment */
  rate = segment->rate * qtdemux->segment.rate;

  /* update the segment values used for clipping */
  gst_segment_init (&stream->segment, GST_FORMAT_TIME);
  gst_segment_set_newsegment (&stream->segment, FALSE, rate, GST_FORMAT_TIME,
      start, stop, time);

  /* now prepare and send the segment */
  if (stream->pad) {
    event = gst_event_new_new_segment (FALSE, rate, GST_FORMAT_TIME,
        start, stop, time);
    gst_pad_push_event (stream->pad, event);
    /* assume we can send more data now */
    stream->last_ret = GST_FLOW_OK;
  }

  /* and move to the keyframe before the indicated media time of the
   * segment */
  if (qtdemux->segment.rate >= 0) {
    index = gst_qtdemux_find_index (qtdemux, stream, start);
    stream->to_sample = stream->n_samples;
    GST_DEBUG_OBJECT (qtdemux, "moving data pointer to %" GST_TIME_FORMAT
        ", index: %u, pts %" GST_TIME_FORMAT, GST_TIME_ARGS (start), index,
        GST_TIME_ARGS (stream->samples[index].timestamp));
  } else {
    index = gst_qtdemux_find_index (qtdemux, stream, stop);
    stream->to_sample = index;
    GST_DEBUG_OBJECT (qtdemux, "moving data pointer to %" GST_TIME_FORMAT
        ", index: %u, pts %" GST_TIME_FORMAT, GST_TIME_ARGS (stop), index,
        GST_TIME_ARGS (stream->samples[index].timestamp));
  }

  /* we're at the right spot */
  if (index == stream->sample_index) {
    GST_DEBUG_OBJECT (qtdemux, "we are at the right index");
    return TRUE;
  }

  /* find keyframe of the target index */
  kf_index = gst_qtdemux_find_keyframe (qtdemux, stream, index);

  /* if we move forwards, we don't have to go back to the previous
   * keyframe since we already sent that. We can also just jump to
   * the keyframe right before the target index if there is one. */
  if (index > stream->sample_index) {
    /* moving forwards check if we move past a keyframe */
    if (kf_index > stream->sample_index) {
      GST_DEBUG_OBJECT (qtdemux, "moving forwards to keyframe at %u (pts %"
          GST_TIME_FORMAT, kf_index,
          GST_TIME_ARGS (stream->samples[kf_index].timestamp));
      gst_qtdemux_move_stream (qtdemux, stream, kf_index);
    } else {
      GST_DEBUG_OBJECT (qtdemux, "moving forwards, keyframe at %u (pts %"
          GST_TIME_FORMAT " already sent", kf_index,
          GST_TIME_ARGS (stream->samples[kf_index].timestamp));
    }
  } else {
    GST_DEBUG_OBJECT (qtdemux, "moving backwards to keyframe at %u (pts %"
        GST_TIME_FORMAT, kf_index,
        GST_TIME_ARGS (stream->samples[kf_index].timestamp));
    gst_qtdemux_move_stream (qtdemux, stream, kf_index);
  }

  return TRUE;
}
