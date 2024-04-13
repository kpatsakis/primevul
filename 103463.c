gst_qtdemux_combine_flows (GstQTDemux * demux, QtDemuxStream * stream,
    GstFlowReturn ret)
{
  gint i;
  gboolean unexpected = FALSE, not_linked = TRUE;

  GST_LOG_OBJECT (demux, "flow return: %s", gst_flow_get_name (ret));

  /* store the value */
  stream->last_ret = ret;

  for (i = 0; i < demux->n_streams; i++) {
    QtDemuxStream *ostream = demux->streams[i];

    ret = ostream->last_ret;

    /* no unexpected or unlinked, return */
    if (ret != GST_FLOW_UNEXPECTED && ret != GST_FLOW_NOT_LINKED)
      goto done;

    /* we check to see if we have at least 1 unexpected or all unlinked */
    unexpected |= (ret == GST_FLOW_UNEXPECTED);
    not_linked &= (ret == GST_FLOW_NOT_LINKED);
  }
  /* when we get here, we all have unlinked or unexpected */
  if (not_linked)
    ret = GST_FLOW_NOT_LINKED;
  else if (unexpected)
    ret = GST_FLOW_UNEXPECTED;
done:
  GST_LOG_OBJECT (demux, "combined flow return: %s", gst_flow_get_name (ret));
  return ret;
}
