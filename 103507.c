qtdemux_tag_add_str (GstQTDemux * qtdemux, const char *tag, const char *dummy,
    GNode * node)
{
  const gchar *env_vars[] = { "GST_QT_TAG_ENCODING", "GST_TAG_ENCODING", NULL };
  GNode *data;
  char *s;
  int len;
  int type;
  int offset;

  data = qtdemux_tree_get_child_by_type (node, FOURCC_data);
  if (data) {
    len = QT_UINT32 (data->data);
    type = QT_UINT32 ((guint8 *) data->data + 8);
    if (type == 0x00000001) {
      s = gst_tag_freeform_string_to_utf8 ((char *) data->data + 16, len - 16,
          env_vars);
      if (s) {
        GST_DEBUG_OBJECT (qtdemux, "adding tag %s", GST_STR_NULL (s));
        gst_tag_list_add (qtdemux->tag_list, GST_TAG_MERGE_REPLACE, tag, s,
            NULL);
        g_free (s);
      } else {
        GST_DEBUG_OBJECT (qtdemux, "failed to convert %s tag to UTF-8", tag);
      }
    }
  } else {
    len = QT_UINT32 (node->data);
    type = QT_UINT32 ((guint8 *) node->data + 4);
    if (type & 0xa9000000) {
      /* Type starts with the (C) symbol, so the next 32 bits are
       * the language code, which we ignore */
      offset = 12;
      GST_DEBUG_OBJECT (qtdemux, "found international text tag");
    } else {
      offset = 8;
      GST_DEBUG_OBJECT (qtdemux, "found normal text tag");
    }
    s = gst_tag_freeform_string_to_utf8 ((char *) node->data + offset,
        len - offset, env_vars);
    if (s) {
      GST_DEBUG_OBJECT (qtdemux, "adding tag %s", GST_STR_NULL (s));
      gst_tag_list_add (qtdemux->tag_list, GST_TAG_MERGE_REPLACE, tag, s, NULL);
      g_free (s);
    } else {
      GST_DEBUG_OBJECT (qtdemux, "failed to convert %s tag to UTF-8", tag);
    }
  }
}
