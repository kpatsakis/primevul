gst_qtdemux_chain (GstPad * sinkpad, GstBuffer * inbuf)
{
  GstQTDemux *demux;
  GstFlowReturn ret = GST_FLOW_OK;

  demux = GST_QTDEMUX (gst_pad_get_parent (sinkpad));

  gst_adapter_push (demux->adapter, inbuf);

  GST_DEBUG_OBJECT (demux, "pushing in inbuf %p, neededbytes:%u, available:%u",
      inbuf, demux->neededbytes, gst_adapter_available (demux->adapter));

  while (((gst_adapter_available (demux->adapter)) >= demux->neededbytes) &&
      (ret == GST_FLOW_OK)) {

    GST_DEBUG_OBJECT (demux,
        "state:%d , demux->neededbytes:%d, demux->offset:%lld", demux->state,
        demux->neededbytes, demux->offset);

    switch (demux->state) {
      case QTDEMUX_STATE_INITIAL:{
        const guint8 *data;
        guint32 fourcc;
        guint64 size;

        data = gst_adapter_peek (demux->adapter, demux->neededbytes);

        /* get fourcc/length, set neededbytes */
        extract_initial_length_and_fourcc ((guint8 *) data, &size, &fourcc);
        GST_DEBUG_OBJECT (demux,
            "Peeking found [%" GST_FOURCC_FORMAT "] size: %u",
            GST_FOURCC_ARGS (fourcc), (guint) size);
        if (size == 0) {
          GST_ELEMENT_ERROR (demux, STREAM, DECODE,
              (_("This file is invalid and cannot be played.")),
              ("initial atom '%" GST_FOURCC_FORMAT "' has empty length",
                  GST_FOURCC_ARGS (fourcc)));
          ret = GST_FLOW_ERROR;
          break;
        }
        if (fourcc == FOURCC_mdat) {
          if (demux->n_streams > 0) {
            demux->state = QTDEMUX_STATE_MOVIE;
            demux->neededbytes = next_entry_size (demux);
          } else {
            demux->state = QTDEMUX_STATE_BUFFER_MDAT;
            demux->neededbytes = size;
            demux->mdatoffset = demux->offset;
          }
        } else {
          demux->neededbytes = size;
          demux->state = QTDEMUX_STATE_HEADER;
        }
        break;
      }
      case QTDEMUX_STATE_HEADER:{
        guint8 *data;
        guint32 fourcc;

        GST_DEBUG_OBJECT (demux, "In header");

        data = gst_adapter_take (demux->adapter, demux->neededbytes);

        /* parse the header */
        extract_initial_length_and_fourcc (data, NULL, &fourcc);
        if (fourcc == FOURCC_moov) {
          GST_DEBUG_OBJECT (demux, "Parsing [moov]");

          qtdemux_parse_moov (demux, data, demux->neededbytes);
          qtdemux_node_dump (demux, demux->moov_node);
          qtdemux_parse_tree (demux);

          g_node_destroy (demux->moov_node);
          g_free (data);
          demux->moov_node = NULL;
        } else {
          GST_WARNING_OBJECT (demux,
              "Unknown fourcc while parsing header : %" GST_FOURCC_FORMAT,
              GST_FOURCC_ARGS (fourcc));
          /* Let's jump that one and go back to initial state */
        }

        GST_DEBUG_OBJECT (demux, "Finished parsing the header");
        if (demux->mdatbuffer && demux->n_streams) {
          /* the mdat was before the header */
          GST_DEBUG_OBJECT (demux, "We have n_streams:%d and mdatbuffer:%p",
              demux->n_streams, demux->mdatbuffer);
          gst_adapter_clear (demux->adapter);
          GST_DEBUG_OBJECT (demux, "mdatbuffer starts with %" GST_FOURCC_FORMAT,
              GST_FOURCC_ARGS (QT_UINT32 (demux->mdatbuffer)));
          gst_adapter_push (demux->adapter, demux->mdatbuffer);
          demux->mdatbuffer = NULL;
          demux->offset = demux->mdatoffset;
          demux->neededbytes = next_entry_size (demux);
          demux->state = QTDEMUX_STATE_MOVIE;
        } else {
          GST_DEBUG_OBJECT (demux, "Carrying on normally");
          demux->offset += demux->neededbytes;
          demux->neededbytes = 16;
          demux->state = QTDEMUX_STATE_INITIAL;
        }

        break;
      }
      case QTDEMUX_STATE_BUFFER_MDAT:{
        GST_DEBUG_OBJECT (demux, "Got our buffer at offset %lld",
            demux->mdatoffset);
        if (demux->mdatbuffer)
          gst_buffer_unref (demux->mdatbuffer);
        demux->mdatbuffer = gst_buffer_new ();
        gst_buffer_set_data (demux->mdatbuffer,
            gst_adapter_take (demux->adapter, demux->neededbytes),
            demux->neededbytes);
        GST_DEBUG_OBJECT (demux, "mdatbuffer starts with %" GST_FOURCC_FORMAT,
            GST_FOURCC_ARGS (QT_UINT32 (demux->mdatbuffer)));
        demux->offset += demux->neededbytes;
        demux->neededbytes = 16;
        demux->state = QTDEMUX_STATE_INITIAL;
        gst_qtdemux_post_progress (demux, 1, 1);

        break;
      }
      case QTDEMUX_STATE_MOVIE:{
        guint8 *data;
        GstBuffer *outbuf;
        QtDemuxStream *stream = NULL;
        int i = -1;

        GST_DEBUG_OBJECT (demux, "BEGIN // in MOVIE for offset %lld",
            demux->offset);

        if (demux->todrop) {
          gst_adapter_flush (demux->adapter, demux->todrop);
          demux->neededbytes -= demux->todrop;
          demux->offset += demux->todrop;
        }

        /* Figure out which stream this is packet belongs to */
        for (i = 0; i < demux->n_streams; i++) {
          stream = demux->streams[i];
          GST_LOG_OBJECT (demux,
              "Checking stream %d (sample_index:%d / offset:%lld / size:%d / chunk:%d)",
              i, stream->sample_index,
              stream->samples[stream->sample_index].offset,
              stream->samples[stream->sample_index].size,
              stream->samples[stream->sample_index].chunk);

          if (stream->samples[stream->sample_index].offset == demux->offset)
            break;
        }

        if (stream == NULL)
          goto unknown_stream;

        /* first buffer? */
        /* FIXME : this should be handled in sink_event */
        if (demux->last_ts == GST_CLOCK_TIME_NONE) {
          gst_qtdemux_push_event (demux,
              gst_event_new_new_segment (FALSE, 1.0, GST_FORMAT_TIME,
                  0, GST_CLOCK_TIME_NONE, 0));
        }

        /* get data */
        data = gst_adapter_take (demux->adapter, demux->neededbytes);

        /* Put data in a buffer, set timestamps, caps, ... */
        outbuf = gst_buffer_new ();
        gst_buffer_set_data (outbuf, data, demux->neededbytes);
        GST_DEBUG_OBJECT (demux, "stream : %" GST_FOURCC_FORMAT,
            GST_FOURCC_ARGS (stream->fourcc));

        if (stream->samples[stream->sample_index].pts_offset) {
          demux->last_ts = stream->samples[stream->sample_index].timestamp;
          GST_BUFFER_TIMESTAMP (outbuf) = demux->last_ts +
              stream->samples[stream->sample_index].pts_offset;
        } else {
          GST_BUFFER_TIMESTAMP (outbuf) =
              stream->samples[stream->sample_index].timestamp;
          demux->last_ts = GST_BUFFER_TIMESTAMP (outbuf);
        }
        GST_BUFFER_DURATION (outbuf) =
            stream->samples[stream->sample_index].duration;

        /* send buffer */
        if (stream->pad) {
          GST_LOG_OBJECT (demux,
              "Pushing buffer with time %" GST_TIME_FORMAT " on pad %p",
              GST_TIME_ARGS (GST_BUFFER_TIMESTAMP (outbuf)), stream->pad);
          gst_buffer_set_caps (outbuf, stream->caps);
          ret = gst_pad_push (stream->pad, outbuf);
        } else {
          gst_buffer_unref (outbuf);
          ret = GST_FLOW_OK;
        }

        /* combine flows */
        ret = gst_qtdemux_combine_flows (demux, stream, ret);

        stream->sample_index++;

        /* update current offset and figure out size of next buffer */
        GST_LOG_OBJECT (demux, "increasing offset %" G_GUINT64_FORMAT " by %u",
            demux->offset, demux->neededbytes);
        demux->offset += demux->neededbytes;
        GST_LOG_OBJECT (demux, "offset is now %lld", demux->offset);

        if ((demux->neededbytes = next_entry_size (demux)) == -1)
          goto eos;
        break;
      }
      default:
        goto invalid_state;
    }
  }

  /* when buffering movie data, at least show user something is happening */
  if (ret == GST_FLOW_OK && demux->state == QTDEMUX_STATE_BUFFER_MDAT &&
      gst_adapter_available (demux->adapter) <= demux->neededbytes) {
    gst_qtdemux_post_progress (demux, gst_adapter_available (demux->adapter),
        demux->neededbytes);
  }
done:
  gst_object_unref (demux);

  return ret;

  /* ERRORS */
unknown_stream:
  {
    GST_ELEMENT_ERROR (demux, STREAM, FAILED, (NULL), ("unknown stream found"));
    ret = GST_FLOW_ERROR;
    goto done;
  }
eos:
  {
    GST_DEBUG_OBJECT (demux, "no next entry, EOS");
    ret = GST_FLOW_UNEXPECTED;
    goto done;
  }
invalid_state:
  {
    GST_ELEMENT_ERROR (demux, STREAM, FAILED,
        (NULL), ("qtdemuxer invalid state %d", demux->state));
    ret = GST_FLOW_ERROR;
    goto done;
  }
}
