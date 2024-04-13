gst_qtdemux_loop_state_header (GstQTDemux * qtdemux)
{
  guint64 length;
  guint32 fourcc;
  GstBuffer *buf = NULL;
  GstFlowReturn ret = GST_FLOW_OK;
  guint64 cur_offset = qtdemux->offset;

  ret = gst_pad_pull_range (qtdemux->sinkpad, cur_offset, 16, &buf);
  if (ret != GST_FLOW_OK)
    goto beach;
  extract_initial_length_and_fourcc (GST_BUFFER_DATA (buf), &length, &fourcc);
  gst_buffer_unref (buf);

  if (length == 0) {
    GST_ELEMENT_ERROR (qtdemux, STREAM, DECODE,
        (_("This file is invalid and cannot be played.")),
        ("Header atom '%" GST_FOURCC_FORMAT "' has empty length",
            GST_FOURCC_ARGS (fourcc)));
    ret = GST_FLOW_ERROR;
    goto beach;
  }

  switch (fourcc) {
    case FOURCC_mdat:
    case FOURCC_free:
    case FOURCC_wide:
    case FOURCC_PICT:
    case FOURCC_pnot:
    {
      GST_LOG_OBJECT (qtdemux,
          "skipping atom '%" GST_FOURCC_FORMAT "' at %" G_GUINT64_FORMAT,
          GST_FOURCC_ARGS (fourcc), cur_offset);
      cur_offset += length;
      qtdemux->offset += length;
      break;
    }
    case FOURCC_moov:
    {
      GstBuffer *moov;

      ret = gst_pad_pull_range (qtdemux->sinkpad, cur_offset, length, &moov);
      if (ret != GST_FLOW_OK)
        goto beach;
      if (length != GST_BUFFER_SIZE (moov)) {
        /* Some files have a 'moov' atom at the end of the file which contains
         * a terminal 'free' atom where the body of the atom is missing.
         * Check for, and permit, this special case.
         */
        if (GST_BUFFER_SIZE (moov) >= 8) {
          guint8 *final_data = GST_BUFFER_DATA (moov) +
              (GST_BUFFER_SIZE (moov) - 8);
          guint32 final_length = QT_UINT32 (final_data);
          guint32 final_fourcc = QT_FOURCC (final_data + 4);
          if (final_fourcc == FOURCC_free &&
              GST_BUFFER_SIZE (moov) + final_length - 8 == length) {
            /* Ok, we've found that special case. Allocate a new buffer with
             * that free atom actually present. */
            GstBuffer *newmoov = gst_buffer_new_and_alloc (length);
            gst_buffer_copy_metadata (newmoov, moov,
                GST_BUFFER_COPY_FLAGS | GST_BUFFER_COPY_TIMESTAMPS |
                GST_BUFFER_COPY_CAPS);
            memcpy (GST_BUFFER_DATA (newmoov), GST_BUFFER_DATA (moov),
                GST_BUFFER_SIZE (moov));
            memset (GST_BUFFER_DATA (newmoov) + GST_BUFFER_SIZE (moov), 0,
                final_length - 8);
            gst_buffer_unref (moov);
            moov = newmoov;
          }
        }
      }

      if (length != GST_BUFFER_SIZE (moov)) {
        GST_ELEMENT_ERROR (qtdemux, STREAM, DECODE,
            (_("This file is incomplete and cannot be played.")),
            ("We got less than expected (received %u, wanted %u, offset %"
                G_GUINT64_FORMAT ")",
                GST_BUFFER_SIZE (moov), (guint) length, cur_offset));
        ret = GST_FLOW_ERROR;
        goto beach;
      }
      cur_offset += length;
      qtdemux->offset += length;

      qtdemux_parse_moov (qtdemux, GST_BUFFER_DATA (moov), length);
      qtdemux_node_dump (qtdemux, qtdemux->moov_node);

      qtdemux_parse_tree (qtdemux);
      g_node_destroy (qtdemux->moov_node);
      gst_buffer_unref (moov);
      qtdemux->moov_node = NULL;
      qtdemux->state = QTDEMUX_STATE_MOVIE;
      GST_DEBUG_OBJECT (qtdemux, "switching state to STATE_MOVIE (%d)",
          qtdemux->state);
      break;
    }
    case FOURCC_ftyp:
    {
      GstBuffer *ftyp;

      /* extract major brand; might come in handy for ISO vs QT issues */
      ret = gst_pad_pull_range (qtdemux->sinkpad, cur_offset, length, &ftyp);
      if (ret != GST_FLOW_OK)
        goto beach;
      cur_offset += length;
      qtdemux->offset += length;
      /* only consider at least a sufficiently complete ftyp atom */
      if (length >= 20) {
        qtdemux->major_brand = QT_FOURCC (GST_BUFFER_DATA (ftyp) + 8);
        GST_DEBUG_OBJECT (qtdemux, "major brand: %" GST_FOURCC_FORMAT,
            GST_FOURCC_ARGS (qtdemux->major_brand));
      }
      gst_buffer_unref (ftyp);
      break;
    }
    default:
    {
      GST_LOG_OBJECT (qtdemux,
          "unknown %08x '%" GST_FOURCC_FORMAT "' at %" G_GUINT64_FORMAT, fourcc,
          GST_FOURCC_ARGS (fourcc), cur_offset);
      cur_offset += length;
      qtdemux->offset += length;
      break;
    }
  }

beach:
  return ret;
}
