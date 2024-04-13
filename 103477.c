gst_qtdemux_loop (GstPad * pad)
{
  GstQTDemux *qtdemux;
  guint64 cur_offset;
  GstFlowReturn ret;

  qtdemux = GST_QTDEMUX (gst_pad_get_parent (pad));

  cur_offset = qtdemux->offset;
  GST_LOG_OBJECT (qtdemux, "loop at position %" G_GUINT64_FORMAT ", state %d",
      cur_offset, qtdemux->state);

  switch (qtdemux->state) {
    case QTDEMUX_STATE_INITIAL:
    case QTDEMUX_STATE_HEADER:
      ret = gst_qtdemux_loop_state_header (qtdemux);
      break;
    case QTDEMUX_STATE_MOVIE:
      ret = gst_qtdemux_loop_state_movie (qtdemux);
      if (qtdemux->segment.rate < 0 && ret == GST_FLOW_UNEXPECTED) {
        ret = gst_qtdemux_seek_to_previous_keyframe (qtdemux);
      }
      break;
    default:
      /* ouch */
      goto invalid_state;
  }

  /* if something went wrong, pause */
  if (ret != GST_FLOW_OK)
    goto pause;

done:
  gst_object_unref (qtdemux);
  return;

  /* ERRORS */
invalid_state:
  {
    GST_ELEMENT_ERROR (qtdemux, STREAM, FAILED,
        (NULL), ("streaming stopped, invalid state"));
    qtdemux->segment_running = FALSE;
    gst_pad_pause_task (pad);
    gst_qtdemux_push_event (qtdemux, gst_event_new_eos ());
    goto done;
  }
pause:
  {
    const gchar *reason = gst_flow_get_name (ret);

    GST_LOG_OBJECT (qtdemux, "pausing task, reason %s", reason);

    qtdemux->segment_running = FALSE;
    gst_pad_pause_task (pad);

    /* fatal errors need special actions */
    if (GST_FLOW_IS_FATAL (ret) || ret == GST_FLOW_NOT_LINKED) {
      /* check EOS */
      if (ret == GST_FLOW_UNEXPECTED) {
        if (qtdemux->n_streams == 0) {
          /* we have no streams, post an error */
          GST_ELEMENT_ERROR (qtdemux, STREAM, DECODE,
              (_("This file contains no playable streams.")),
              ("no known streams found"));
        }
        if (qtdemux->segment.flags & GST_SEEK_FLAG_SEGMENT) {
          gint64 stop;

          /* FIXME: I am not sure this is the right fix. If the sinks are
           * supposed to detect the segment is complete and accumulate 
           * automatically, it does not seem to work here. Need more work */
          qtdemux->segment_running = TRUE;

          if ((stop = qtdemux->segment.stop) == -1)
            stop = qtdemux->segment.duration;

          if (qtdemux->segment.rate >= 0) {
            GST_LOG_OBJECT (qtdemux, "Sending segment done, at end of segment");
            gst_element_post_message (GST_ELEMENT_CAST (qtdemux),
                gst_message_new_segment_done (GST_OBJECT_CAST (qtdemux),
                    GST_FORMAT_TIME, stop));
          } else {
            /*  For Reverse Playback */
            GST_LOG_OBJECT (qtdemux,
                "Sending segment done, at start of segment");
            gst_element_post_message (GST_ELEMENT_CAST (qtdemux),
                gst_message_new_segment_done (GST_OBJECT_CAST (qtdemux),
                    GST_FORMAT_TIME, qtdemux->segment.start));
          }
        } else {
          GST_LOG_OBJECT (qtdemux, "Sending EOS at end of segment");
          gst_qtdemux_push_event (qtdemux, gst_event_new_eos ());
        }
      } else {
        GST_ELEMENT_ERROR (qtdemux, STREAM, FAILED,
            (NULL), ("streaming stopped, reason %s", reason));
        gst_qtdemux_push_event (qtdemux, gst_event_new_eos ());
      }
    }
    goto done;
  }
}
