qtdemux_parse_node (GstQTDemux * qtdemux, GNode * node, guint8 * buffer,
    int length)
{
  guint32 fourcc;
  guint32 node_length;
  const QtNodeType *type;
  guint8 *end;

  GST_LOG_OBJECT (qtdemux, "qtdemux_parse buffer %p length %d", buffer, length);

  node_length = QT_UINT32 (buffer);
  fourcc = QT_FOURCC (buffer + 4);

  type = qtdemux_type_get (fourcc);

  /* ignore empty nodes */
  if (fourcc == 0 || node_length == 8)
    return TRUE;

  end = buffer + length;

  GST_LOG_OBJECT (qtdemux,
      "parsing '%" GST_FOURCC_FORMAT "', length=%d, name '%s'",
      GST_FOURCC_ARGS (fourcc), node_length, type->name);

  if (type->flags & QT_FLAG_CONTAINER) {
    qtdemux_parse_container (qtdemux, node, buffer + 8, end);
  } else {
    switch (fourcc) {
      case FOURCC_stsd:
      {
        if (node_length < 20) {
          GST_LOG_OBJECT (qtdemux, "skipping small stsd box");
          break;
        }
        GST_DEBUG_OBJECT (qtdemux,
            "parsing stsd (sample table, sample description) atom");
        qtdemux_parse_container (qtdemux, node, buffer + 16, end);
        break;
      }
      case FOURCC_mp4a:
      {
        guint32 version;
        guint32 offset;

        if (length < 20) {
          /* small boxes are also inside wave inside the mp4a box */
          GST_LOG_OBJECT (qtdemux, "skipping small mp4a box");
          break;
        }
        version = QT_UINT32 (buffer + 16);

        GST_WARNING_OBJECT (qtdemux, "mp4a version 0x%08x", version);

        /* parse any esds descriptors */
        switch (version) {
          case 0x00000000:
            offset = 0x24;
            break;
          case 0x00010000:
            offset = 0x34;
            break;
          case 0x00020000:
            offset = 0x58;
            break;
          default:
            GST_WARNING_OBJECT (qtdemux, "unhandled mp4a version 0x%08x",
                version);
            offset = 0;
            break;
        }
        if (offset)
          qtdemux_parse_container (qtdemux, node, buffer + offset, end);
        break;
      }
      case FOURCC_mp4v:
      {
        guint8 *buf;
        guint32 version;
        int tlen;

        GST_DEBUG_OBJECT (qtdemux, "parsing in mp4v");
        version = QT_UINT32 (buffer + 16);
        GST_DEBUG_OBJECT (qtdemux, "version %08x", version);
        if (1 || version == 0x00000000) {
          buf = buffer + 0x32;

          /* FIXME Quicktime uses PASCAL string while
           * the iso format uses C strings. Check the file
           * type before attempting to parse the string here. */
          tlen = QT_UINT8 (buf);
          GST_DEBUG_OBJECT (qtdemux, "tlen = %d", tlen);
          buf++;
          GST_DEBUG_OBJECT (qtdemux, "string = %.*s", tlen, (char *) buf);
          /* the string has a reserved space of 32 bytes so skip
           * the remaining 31 */
          buf += 31;
          buf += 4;             /* and 4 bytes reserved */

          qtdemux_dump_mem (buf, end - buf);

          qtdemux_parse_container (qtdemux, node, buf, end);
        }
        break;
      }
      case FOURCC_mjp2:
      {
        qtdemux_parse_container (qtdemux, node, buffer + 86, end);
        break;
      }
      case FOURCC_meta:
      {
        GST_DEBUG_OBJECT (qtdemux, "parsing meta atom");
        qtdemux_parse_container (qtdemux, node, buffer + 12, end);
        break;
      }
      case FOURCC_XiTh:
      {
        guint32 version;
        guint32 offset;

        version = QT_UINT32 (buffer + 12);
        GST_DEBUG_OBJECT (qtdemux, "parsing XiTh atom version 0x%08x", version);

        switch (version) {
          case 0x00000001:
            offset = 0x62;
            break;
          default:
            GST_DEBUG_OBJECT (qtdemux, "unknown version 0x%08x", version);
            offset = 0;
            break;
        }
        if (offset)
          qtdemux_parse_container (qtdemux, node, buffer + offset, end);
        break;
      }
      default:
        break;
    }
  }
  GST_LOG_OBJECT (qtdemux, "parsed '%" GST_FOURCC_FORMAT,
      GST_FOURCC_ARGS (fourcc));
  return TRUE;
}
