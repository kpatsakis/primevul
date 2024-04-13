next_entry_size (GstQTDemux * demux)
{
  QtDemuxStream *stream;
  int i;
  int smallidx = -1;
  guint64 smalloffs = (guint64) - 1;

  GST_LOG_OBJECT (demux, "Finding entry at offset %lld", demux->offset);

  for (i = 0; i < demux->n_streams; i++) {
    stream = demux->streams[i];

    if (stream->sample_index == -1)
      stream->sample_index = 0;

    GST_LOG_OBJECT (demux,
        "Checking Stream %d (sample_index:%d / offset:%lld / size:%d / chunk:%d)",
        i, stream->sample_index, stream->samples[stream->sample_index].offset,
        stream->samples[stream->sample_index].size,
        stream->samples[stream->sample_index].chunk);

    if (((smalloffs == -1)
            || (stream->samples[stream->sample_index].offset < smalloffs))
        && (stream->samples[stream->sample_index].size)) {
      smallidx = i;
      smalloffs = stream->samples[stream->sample_index].offset;
    }
  }

  GST_LOG_OBJECT (demux, "stream %d offset %lld demux->offset :%lld",
      smallidx, smalloffs, demux->offset);

  if (smallidx == -1)
    return -1;
  stream = demux->streams[smallidx];

  if (stream->samples[stream->sample_index].offset >= demux->offset) {
    demux->todrop =
        stream->samples[stream->sample_index].offset - demux->offset;
    return stream->samples[stream->sample_index].size + demux->todrop;
  }

  GST_DEBUG_OBJECT (demux, "There wasn't any entry at offset %lld",
      demux->offset);
  return -1;
}
