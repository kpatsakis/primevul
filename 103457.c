gst_qtdemux_advance_sample (GstQTDemux * qtdemux, QtDemuxStream * stream)
{
  QtDemuxSample *sample;
  QtDemuxSegment *segment;

  if (stream->sample_index >= stream->to_sample) {
    /* Mark the stream as EOS */
    GST_DEBUG_OBJECT (qtdemux, "reached max allowed sample %u, mark EOS",
        stream->to_sample);
    stream->time_position = -1;
    return;
  }

  /* move to next sample */
  stream->sample_index++;

  /* get current segment */
  segment = &stream->segments[stream->segment_index];

  /* reached the last sample, we need the next segment */
  if (stream->sample_index >= stream->n_samples)
    goto next_segment;

  /* get next sample */
  sample = &stream->samples[stream->sample_index];

  /* see if we are past the segment */
  if (sample->timestamp >= segment->media_stop)
    goto next_segment;

  if (sample->timestamp >= segment->media_start) {
    /* inside the segment, update time_position, looks very familiar to
     * GStreamer segments, doesn't it? */
    stream->time_position =
        (sample->timestamp - segment->media_start) + segment->time;
  } else {
    /* not yet in segment, time does not yet increment. This means
     * that we are still prerolling keyframes to the decoder so it can
     * decode the first sample of the segment. */
    stream->time_position = segment->time;
  }
  return;

  /* move to the next segment */
next_segment:
  {
    GST_DEBUG_OBJECT (qtdemux, "segment %d ended ", stream->segment_index);

    if (stream->segment_index == stream->n_segments - 1) {
      /* are we at the end of the last segment, we're EOS */
      stream->time_position = -1;
    } else {
      /* else we're only at the end of the current segment */
      stream->time_position = segment->stop_time;
    }
    /* make sure we select a new segment */
    stream->segment_index = -1;
  }
}
