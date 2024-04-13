gst_qtdemux_add_stream (GstQTDemux * qtdemux,
    QtDemuxStream * stream, GstTagList * list)
{
  if (qtdemux->n_streams >= GST_QTDEMUX_MAX_STREAMS)
    goto too_many_streams;

  if (stream->subtype == FOURCC_vide) {
    gchar *name = g_strdup_printf ("video_%02d", qtdemux->n_video_streams);

    stream->pad =
        gst_pad_new_from_static_template (&gst_qtdemux_videosrc_template, name);
    g_free (name);

    /* fps is calculated base on the duration of the first frames since
     * qt does not have a fixed framerate. */
    if ((stream->n_samples == 1) && (stream->min_duration == 0)) {
      /* still frame */
      stream->fps_n = 0;
      stream->fps_d = 1;
    } else {
      stream->fps_n = stream->timescale;
      if (stream->min_duration == 0)
        stream->fps_d = 1;
      else
        stream->fps_d = stream->min_duration;
    }

    if (stream->caps) {
      gboolean gray;
      gint depth, palette_count;
      const guint32 *palette_data = NULL;

      gst_caps_set_simple (stream->caps,
          "width", G_TYPE_INT, stream->width,
          "height", G_TYPE_INT, stream->height,
          "framerate", GST_TYPE_FRACTION, stream->fps_n, stream->fps_d, NULL);

      depth = stream->bits_per_sample;

      /* more than 32 bits means grayscale */
      gray = (depth > 32);
      /* low 32 bits specify the depth  */
      depth &= 0x1F;

      /* different number of palette entries is determined by depth. */
      palette_count = 0;
      if ((depth == 1) || (depth == 2) || (depth == 4) || (depth == 8))
        palette_count = (1 << depth);

      switch (palette_count) {
        case 0:
          break;
        case 2:
          palette_data = ff_qt_default_palette_2;
          break;
        case 4:
          palette_data = ff_qt_default_palette_4;
          break;
        case 16:
          if (gray)
            palette_data = ff_qt_grayscale_palette_16;
          else
            palette_data = ff_qt_default_palette_16;
          break;
        case 256:
          if (gray)
            palette_data = ff_qt_grayscale_palette_256;
          else
            palette_data = ff_qt_default_palette_256;
          break;
        default:
          GST_ELEMENT_WARNING (qtdemux, STREAM, DECODE,
              (_("The video in this file might not play correctly.")),
              ("unsupported palette depth %d", depth));
          break;
      }
      if (palette_data) {
        GstBuffer *palette;

        /* make sure it's not writable. We leave MALLOCDATA to NULL so that we
         * don't free any of the buffer data. */
        palette = gst_buffer_new ();
        GST_BUFFER_FLAG_SET (palette, GST_BUFFER_FLAG_READONLY);
        GST_BUFFER_DATA (palette) = (guint8 *) palette_data;
        GST_BUFFER_SIZE (palette) = sizeof (guint32) * palette_count;

        gst_caps_set_simple (stream->caps, "palette_data",
            GST_TYPE_BUFFER, palette, NULL);
        gst_buffer_unref (palette);
      } else if (palette_count != 0) {
        GST_ELEMENT_WARNING (qtdemux, STREAM, NOT_IMPLEMENTED,
            (NULL), ("Unsupported palette depth %d. Ignoring stream.", depth));

        gst_object_unref (stream->pad);
        stream->pad = NULL;
      }
    }
    qtdemux->n_video_streams++;
  } else if (stream->subtype == FOURCC_soun) {
    gchar *name = g_strdup_printf ("audio_%02d", qtdemux->n_audio_streams);

    stream->pad =
        gst_pad_new_from_static_template (&gst_qtdemux_audiosrc_template, name);
    g_free (name);
    if (stream->caps) {
      gst_caps_set_simple (stream->caps,
          "rate", G_TYPE_INT, (int) stream->rate,
          "channels", G_TYPE_INT, stream->n_channels, NULL);
    }
    qtdemux->n_audio_streams++;
  } else if (stream->subtype == FOURCC_strm) {
    GST_DEBUG_OBJECT (qtdemux, "stream type, not creating pad");
  } else {
    GST_DEBUG_OBJECT (qtdemux, "unknown stream type");
    goto done;
  }

  qtdemux->streams[qtdemux->n_streams] = stream;
  qtdemux->n_streams++;
  GST_DEBUG_OBJECT (qtdemux, "n_streams is now %d", qtdemux->n_streams);

  if (stream->pad) {
    GST_PAD_ELEMENT_PRIVATE (stream->pad) = stream;

    gst_pad_use_fixed_caps (stream->pad);
    gst_pad_set_event_function (stream->pad, gst_qtdemux_handle_src_event);
    gst_pad_set_query_type_function (stream->pad,
        gst_qtdemux_get_src_query_types);
    gst_pad_set_query_function (stream->pad, gst_qtdemux_handle_src_query);

    GST_DEBUG_OBJECT (qtdemux, "setting caps %" GST_PTR_FORMAT, stream->caps);
    gst_pad_set_caps (stream->pad, stream->caps);

    GST_DEBUG_OBJECT (qtdemux, "adding pad %s %p to qtdemux %p",
        GST_OBJECT_NAME (stream->pad), stream->pad, qtdemux);
    gst_pad_set_active (stream->pad, TRUE);
    gst_element_add_pad (GST_ELEMENT_CAST (qtdemux), stream->pad);
    if (list)
      gst_element_found_tags_for_pad (GST_ELEMENT_CAST (qtdemux), stream->pad,
          list);
  }
done:
  return TRUE;

too_many_streams:
  {
    GST_ELEMENT_WARNING (qtdemux, STREAM, DECODE,
        (_("This file contains too many streams. Only playing first %d"),
            GST_QTDEMUX_MAX_STREAMS), (NULL));
    return TRUE;
  }
}
