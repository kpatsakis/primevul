qtdemux_tag_add_blob (GNode * node, GstQTDemux * demux)
{
  gint len;
  guint8 *data;
  GstBuffer *buf;
  gchar *media_type, *style;
  GstCaps *caps;

  data = node->data;
  len = QT_UINT32 (data);
  buf = gst_buffer_new_and_alloc (len);
  memcpy (GST_BUFFER_DATA (buf), data, len);

  /* heuristic to determine style of tag */
  if (QT_FOURCC (data + 4) == FOURCC_____ ||
      (len > 8 + 12 && QT_FOURCC (data + 12) == FOURCC_data))
    style = "itunes";
  else if (demux->major_brand == GST_MAKE_FOURCC ('q', 't', ' ', ' '))
    style = "quicktime";
  /* fall back to assuming iso/3gp tag style */
  else
    style = "iso";

  media_type = g_strdup_printf ("application/x-gst-qt-%c%c%c%c-tag",
      g_ascii_tolower (data[4]), g_ascii_tolower (data[5]),
      g_ascii_tolower (data[6]), g_ascii_tolower (data[7]));
  caps = gst_caps_new_simple (media_type, "style", G_TYPE_STRING, style, NULL);
  gst_buffer_set_caps (buf, caps);
  gst_caps_unref (caps);
  g_free (media_type);

  GST_DEBUG_OBJECT (demux, "adding private tag; size %d, caps %" GST_PTR_FORMAT,
      GST_BUFFER_SIZE (buf), caps);

  gst_tag_list_add (demux->tag_list, GST_TAG_MERGE_APPEND,
      GST_QT_DEMUX_PRIVATE_TAG, buf, NULL);
  gst_buffer_unref (buf);
}
