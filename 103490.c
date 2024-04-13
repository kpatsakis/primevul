qtdemux_parse_container (GstQTDemux * qtdemux, GNode * node, guint8 * buf,
    guint8 * end)
{
  while (buf < end) {
    GNode *child;
    guint32 len;

    if (buf + 4 > end) {
      GST_LOG_OBJECT (qtdemux, "buffer overrun");
      break;
    }
    len = QT_UINT32 (buf);
    if (len == 0) {
      GST_LOG_OBJECT (qtdemux, "empty container");
      break;
    }
    if (len < 8) {
      GST_WARNING_OBJECT (qtdemux, "length too short (%d < 8)", len);
      break;
    }
    if (len > (end - buf)) {
      GST_WARNING_OBJECT (qtdemux, "length too long (%d > %d)", len, end - buf);
      break;
    }

    child = g_node_new (buf);
    g_node_append (node, child);
    qtdemux_parse_node (qtdemux, child, buf, len);

    buf += len;
  }
  return TRUE;
}
