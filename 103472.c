gst_qtdemux_handle_esds (GstQTDemux * qtdemux, QtDemuxStream * stream,
    GNode * esds, GstTagList * list)
{
  int len = QT_UINT32 (esds->data);
  guint8 *ptr = esds->data;
  guint8 *end = ptr + len;
  int tag;
  guint8 *data_ptr = NULL;
  int data_len = 0;
  guint8 object_type_id = 0;

  qtdemux_dump_mem (ptr, len);
  ptr += 8;
  GST_DEBUG_OBJECT (qtdemux, "version/flags = %08x", QT_UINT32 (ptr));
  ptr += 4;
  while (ptr < end) {
    tag = QT_UINT8 (ptr);
    GST_DEBUG_OBJECT (qtdemux, "tag = %02x", tag);
    ptr++;
    len = get_size (ptr, &ptr);
    GST_DEBUG_OBJECT (qtdemux, "len = %d", len);

    switch (tag) {
      case 0x03:
        GST_DEBUG_OBJECT (qtdemux, "ID %04x", QT_UINT16 (ptr));
        GST_DEBUG_OBJECT (qtdemux, "priority %04x", QT_UINT8 (ptr + 2));
        ptr += 3;
        break;
      case 0x04:
        object_type_id = QT_UINT8 (ptr);
        GST_DEBUG_OBJECT (qtdemux, "object_type_id %02x", object_type_id);
        GST_DEBUG_OBJECT (qtdemux, "stream_type %02x", QT_UINT8 (ptr + 1));
        GST_DEBUG_OBJECT (qtdemux, "buffer_size_db %02x", QT_UINT24 (ptr + 2));
        GST_DEBUG_OBJECT (qtdemux, "max bitrate %d", QT_UINT32 (ptr + 5));
        GST_DEBUG_OBJECT (qtdemux, "avg bitrate %d", QT_UINT32 (ptr + 9));
        ptr += 13;
        break;
      case 0x05:
        GST_DEBUG_OBJECT (qtdemux, "data:");
        qtdemux_dump_mem (ptr, len);
        data_ptr = ptr;
        data_len = len;
        ptr += len;
        break;
      case 0x06:
        GST_DEBUG_OBJECT (qtdemux, "data %02x", QT_UINT8 (ptr));
        ptr += 1;
        break;
      default:
        GST_ERROR_OBJECT (qtdemux, "parse error");
    }
  }

  if (data_ptr) {
    GstBuffer *buffer;

    buffer = gst_buffer_new_and_alloc (data_len);
    memcpy (GST_BUFFER_DATA (buffer), data_ptr, data_len);
    qtdemux_dump_mem (GST_BUFFER_DATA (buffer), data_len);

    GST_DEBUG_OBJECT (qtdemux, "setting codec_data from esds");

    gst_caps_set_simple (stream->caps, "codec_data", GST_TYPE_BUFFER,
        buffer, NULL);
    gst_buffer_unref (buffer);
  }
  /* object_type_id in the stsd atom in mp4a tells us about AAC or plain
   * MPEG audio and other formats */
  switch (object_type_id) {
    case 107:
      /* change to mpeg1 layer 3 audio */
      gst_caps_set_simple (stream->caps, "layer", G_TYPE_INT, 3,
          "mpegversion", G_TYPE_INT, 1, NULL);
      if (list)
        gst_tag_list_add (list, GST_TAG_MERGE_REPLACE,
            GST_TAG_AUDIO_CODEC, "MPEG-1 layer 3", NULL);
      break;
    case 0xE1:
    {
      GstStructure *structure;

      /* QCELP, the codec_data is a riff tag (little endian) with
       * more info (http://ftp.3gpp2.org/TSGC/Working/2003/2003-05-SanDiego/TSG-C-2003-05-San%20Diego/WG1/SWG12/C12-20030512-006%20=%20C12-20030217-015_Draft_Baseline%20Text%20of%20FFMS_R2.doc). */
      structure = gst_caps_get_structure (stream->caps, 0);
      gst_structure_set_name (structure, "audio/qcelp");
      gst_structure_remove_fields (structure, "mpegversion", "framed", NULL);

      if (list)
        gst_tag_list_add (list, GST_TAG_MERGE_REPLACE,
            GST_TAG_AUDIO_CODEC, "QCELP", NULL);
      break;
    }
    default:
      break;
  }
}
