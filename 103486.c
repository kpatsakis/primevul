gst_qtdemux_seek_to_previous_keyframe (GstQTDemux * qtdemux)
{
  guint8 n = 0;
  guint32 seg_idx = 0, k_index = 0;
  guint64 k_pos = 0, last_stop = 0;
  QtDemuxSegment *seg = NULL;
  QtDemuxStream *ref_str = NULL;

  /* Now we choose an arbitrary stream, get the previous keyframe timestamp
   * and finally align all the other streams on that timestamp with their 
   * respective keyframes */
  for (n = 0; n < qtdemux->n_streams; n++) {
    QtDemuxStream *str = qtdemux->streams[n];

    seg_idx = gst_qtdemux_find_segment (qtdemux, str,
        qtdemux->segment.last_stop);

    /* segment not found, continue with normal flow */
    if (seg_idx == -1)
      continue;

    /* No candidate yet, take that one */
    if (!ref_str) {
      ref_str = str;
      continue;
    }

    /* So that stream has a segment, we prefer video streams */
    if (str->subtype == FOURCC_vide) {
      ref_str = str;
      break;
    }
  }

  if (G_UNLIKELY (!ref_str)) {
    GST_DEBUG_OBJECT (qtdemux, "couldn't find any stream");
    goto eos;
  }

  if (G_UNLIKELY (!ref_str->from_sample)) {
    GST_DEBUG_OBJECT (qtdemux, "reached the beginning of the file");
    goto eos;
  }

  /* So that stream has been playing from from_sample to to_sample. We will
   * get the timestamp of the previous sample and search for a keyframe before
   * that. For audio streams we do an arbitrary jump in the past (10 samples) */
  if (ref_str->subtype == FOURCC_vide) {
    k_index = gst_qtdemux_find_keyframe (qtdemux, ref_str,
        ref_str->from_sample - 1);
  } else {
    k_index = ref_str->from_sample - 10;
  }

  /* get current segment for that stream */
  seg = &ref_str->segments[ref_str->segment_index];
  /* Crawl back through segments to find the one containing this I frame */
  while (ref_str->samples[k_index].timestamp < seg->media_start) {
    GST_DEBUG_OBJECT (qtdemux, "keyframe position is out of segment %u",
        ref_str->segment_index);
    if (G_UNLIKELY (!ref_str->segment_index)) {
      /* Reached first segment, let's consider it's EOS */
      goto eos;
    }
    ref_str->segment_index--;
    seg = &ref_str->segments[ref_str->segment_index];
  }
  /* Calculate time position of the keyframe and where we should stop */
  k_pos = (ref_str->samples[k_index].timestamp - seg->media_start) + seg->time;
  last_stop = ref_str->samples[ref_str->from_sample].timestamp;
  last_stop = (last_stop - seg->media_start) + seg->time;

  GST_DEBUG_OBJECT (qtdemux, "preferred stream played from sample %u, "
      "now going to sample %u (pts %" GST_TIME_FORMAT ")", ref_str->from_sample,
      k_index, GST_TIME_ARGS (k_pos));

  /* Set last_stop with the keyframe timestamp we pushed of that stream */
  gst_segment_set_last_stop (&qtdemux->segment, GST_FORMAT_TIME, last_stop);
  GST_DEBUG_OBJECT (qtdemux, "last_stop now is %" GST_TIME_FORMAT,
      GST_TIME_ARGS (last_stop));

  if (G_UNLIKELY (last_stop < qtdemux->segment.start)) {
    GST_DEBUG_OBJECT (qtdemux, "reached the beginning of segment");
    goto eos;
  }

  /* Align them all on this */
  for (n = 0; n < qtdemux->n_streams; n++) {
    guint32 index = 0;
    guint64 media_start = 0, seg_time = 0;
    QtDemuxStream *str = qtdemux->streams[n];

    seg_idx = gst_qtdemux_find_segment (qtdemux, str, k_pos);
    GST_DEBUG_OBJECT (qtdemux, "align segment %d", seg_idx);

    /* segment not found, continue with normal flow */
    if (seg_idx == -1)
      continue;

    /* get segment and time in the segment */
    seg = &str->segments[seg_idx];
    seg_time = k_pos - seg->time;

    /* get the media time in the segment */
    media_start = seg->media_start + seg_time;

    /* get the index of the sample with media time */
    index = gst_qtdemux_find_index (qtdemux, str, media_start);
    GST_DEBUG_OBJECT (qtdemux, "sample for %" GST_TIME_FORMAT " at %u",
        GST_TIME_ARGS (media_start), index);

    /* find previous keyframe */
    k_index = gst_qtdemux_find_keyframe (qtdemux, str, index);

    /* Remember until where we want to go */
    str->to_sample = str->from_sample - 1;
    /* Define our time position */
    str->time_position =
        (str->samples[k_index].timestamp - seg->media_start) + seg->time;
    /* Now seek back in time */
    gst_qtdemux_move_stream (qtdemux, str, k_index);
    GST_DEBUG_OBJECT (qtdemux, "keyframe at %u, time position %"
        GST_TIME_FORMAT " playing from sample %u to %u", k_index,
        GST_TIME_ARGS (str->time_position), str->from_sample, str->to_sample);
  }

  return GST_FLOW_OK;

eos:
  return GST_FLOW_UNEXPECTED;
}
