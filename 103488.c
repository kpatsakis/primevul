qtdemux_audio_caps (GstQTDemux * qtdemux, QtDemuxStream * stream,
    guint32 fourcc, const guint8 * data, int len, gchar ** codec_name)
{
  GstCaps *caps;
  const GstStructure *s;
  const gchar *name;
  gint endian = 0;

  GST_DEBUG_OBJECT (qtdemux, "resolve fourcc %08x", fourcc);

  switch (fourcc) {
    case GST_MAKE_FOURCC ('N', 'O', 'N', 'E'):
    case GST_MAKE_FOURCC ('r', 'a', 'w', ' '):
      _codec ("Raw 8-bit PCM audio");
      caps = gst_caps_new_simple ("audio/x-raw-int", "width", G_TYPE_INT, 8,
          "depth", G_TYPE_INT, 8, "signed", G_TYPE_BOOLEAN, FALSE, NULL);
      break;
    case GST_MAKE_FOURCC ('t', 'w', 'o', 's'):
      endian = G_BIG_ENDIAN;
      /* fall-through */
    case GST_MAKE_FOURCC ('s', 'o', 'w', 't'):
    {
      gchar *str;
      gint depth;

      if (!endian)
        endian = G_LITTLE_ENDIAN;

      depth = stream->bytes_per_packet * 8;
      str = g_strdup_printf ("Raw %d-bit PCM audio", depth);
      _codec (str);
      g_free (str);
      caps = gst_caps_new_simple ("audio/x-raw-int",
          "width", G_TYPE_INT, depth, "depth", G_TYPE_INT, depth,
          "endianness", G_TYPE_INT, endian,
          "signed", G_TYPE_BOOLEAN, TRUE, NULL);
      break;
    }
    case GST_MAKE_FOURCC ('f', 'l', '6', '4'):
      _codec ("Raw 64-bit floating-point audio");
      caps = gst_caps_new_simple ("audio/x-raw-float", "width", G_TYPE_INT, 64,
          "endianness", G_TYPE_INT, G_BIG_ENDIAN, NULL);
      break;
    case GST_MAKE_FOURCC ('f', 'l', '3', '2'):
      _codec ("Raw 32-bit floating-point audio");
      caps = gst_caps_new_simple ("audio/x-raw-float", "width", G_TYPE_INT, 32,
          "endianness", G_TYPE_INT, G_BIG_ENDIAN, NULL);
      break;
    case GST_MAKE_FOURCC ('i', 'n', '2', '4'):
      _codec ("Raw 24-bit PCM audio");
      caps = gst_caps_new_simple ("audio/x-raw-int", "width", G_TYPE_INT, 24,
          "depth", G_TYPE_INT, 24,
          "endianness", G_TYPE_INT, G_BIG_ENDIAN,
          "signed", G_TYPE_BOOLEAN, TRUE, NULL);
      break;
    case GST_MAKE_FOURCC ('i', 'n', '3', '2'):
      _codec ("Raw 32-bit PCM audio");
      caps = gst_caps_new_simple ("audio/x-raw-int", "width", G_TYPE_INT, 32,
          "depth", G_TYPE_INT, 32,
          "endianness", G_TYPE_INT, G_BIG_ENDIAN,
          "signed", G_TYPE_BOOLEAN, TRUE, NULL);
      break;
    case GST_MAKE_FOURCC ('u', 'l', 'a', 'w'):
      _codec ("Mu-law audio");
      caps = gst_caps_new_simple ("audio/x-mulaw", NULL);
      break;
    case GST_MAKE_FOURCC ('a', 'l', 'a', 'w'):
      _codec ("A-law audio");
      caps = gst_caps_new_simple ("audio/x-alaw", NULL);
      break;
    case 0x0200736d:
    case 0x6d730002:
      _codec ("Microsoft ADPCM");
      /* Microsoft ADPCM-ACM code 2 */
      caps = gst_caps_new_simple ("audio/x-adpcm",
          "layout", G_TYPE_STRING, "microsoft", NULL);
      break;
    case 0x1100736d:
    case 0x6d730011:
      _codec ("IMA Loki SDL MJPEG ADPCM");
      /* Loki ADPCM, See #550288 for a file that only decodes
       * with the smjpeg variant of the ADPCM decoder. */
      caps = gst_caps_new_simple ("audio/x-adpcm",
          "layout", G_TYPE_STRING, "smjpeg", NULL);
      break;
    case 0x1700736d:
    case 0x6d730017:
      _codec ("DVI/Intel IMA ADPCM");
      /* FIXME DVI/Intel IMA ADPCM/ACM code 17 */
      caps = gst_caps_new_simple ("audio/x-adpcm",
          "layout", G_TYPE_STRING, "quicktime", NULL);
      break;
    case 0x5500736d:
    case 0x6d730055:
      /* MPEG layer 3, CBR only (pre QT4.1) */
    case GST_MAKE_FOURCC ('.', 'm', 'p', '3'):
      _codec ("MPEG-1 layer 3");
      /* MPEG layer 3, CBR & VBR (QT4.1 and later) */
      caps = gst_caps_new_simple ("audio/mpeg", "layer", G_TYPE_INT, 3,
          "mpegversion", G_TYPE_INT, 1, NULL);
      break;
    case 0x20736d:
      _codec ("AC-3 audio");
      caps = gst_caps_new_simple ("audio/x-ac3", NULL);
      break;
    case GST_MAKE_FOURCC ('M', 'A', 'C', '3'):
      _codec ("MACE-3");
      caps = gst_caps_new_simple ("audio/x-mace",
          "maceversion", G_TYPE_INT, 3, NULL);
      break;
    case GST_MAKE_FOURCC ('M', 'A', 'C', '6'):
      _codec ("MACE-6");
      caps = gst_caps_new_simple ("audio/x-mace",
          "maceversion", G_TYPE_INT, 6, NULL);
      break;
    case GST_MAKE_FOURCC ('O', 'g', 'g', 'V'):
      /* ogg/vorbis */
      caps = gst_caps_new_simple ("application/ogg", NULL);
      break;
    case GST_MAKE_FOURCC ('d', 'v', 'c', 'a'):
      _codec ("DV audio");
      caps = gst_caps_new_simple ("audio/x-dv", NULL);
      break;
    case GST_MAKE_FOURCC ('m', 'p', '4', 'a'):
      _codec ("MPEG-4 AAC audio");
      caps = gst_caps_new_simple ("audio/mpeg",
          "mpegversion", G_TYPE_INT, 4, "framed", G_TYPE_BOOLEAN, TRUE, NULL);
      break;
    case GST_MAKE_FOURCC ('Q', 'D', 'M', 'C'):
      _codec ("QDesign Music");
      caps = gst_caps_new_simple ("audio/x-qdm", NULL);
      break;
    case GST_MAKE_FOURCC ('Q', 'D', 'M', '2'):
      _codec ("QDesign Music v.2");
      /* FIXME: QDesign music version 2 (no constant) */
      if (data) {
        caps = gst_caps_new_simple ("audio/x-qdm2",
            "framesize", G_TYPE_INT, QT_UINT32 (data + 52),
            "bitrate", G_TYPE_INT, QT_UINT32 (data + 40),
            "blocksize", G_TYPE_INT, QT_UINT32 (data + 44), NULL);
      } else {
        caps = gst_caps_new_simple ("audio/x-qdm2", NULL);
      }
      break;
    case GST_MAKE_FOURCC ('a', 'g', 's', 'm'):
      _codec ("GSM audio");
      caps = gst_caps_new_simple ("audio/x-gsm", NULL);
      break;
    case GST_MAKE_FOURCC ('s', 'a', 'm', 'r'):
      _codec ("AMR audio");
      caps = gst_caps_new_simple ("audio/AMR", NULL);
      break;
    case GST_MAKE_FOURCC ('s', 'a', 'w', 'b'):
      _codec ("AMR-WB audio");
      caps = gst_caps_new_simple ("audio/AMR-WB", NULL);
      break;
    case GST_MAKE_FOURCC ('i', 'm', 'a', '4'):
      _codec ("Quicktime IMA ADPCM");
      caps = gst_caps_new_simple ("audio/x-adpcm",
          "layout", G_TYPE_STRING, "quicktime", NULL);
      break;
    case GST_MAKE_FOURCC ('a', 'l', 'a', 'c'):
      _codec ("Apple lossless audio");
      caps = gst_caps_new_simple ("audio/x-alac", NULL);
      break;
    case GST_MAKE_FOURCC ('q', 't', 'v', 'r'):
      /* ? */
    case GST_MAKE_FOURCC ('Q', 'c', 'l', 'p'):
      /* QUALCOMM PureVoice */
    default:
    {
      char *s;

      s = g_strdup_printf ("audio/x-gst-fourcc-%" GST_FOURCC_FORMAT,
          GST_FOURCC_ARGS (fourcc));
      caps = gst_caps_new_simple (s, NULL);
      break;
    }
  }

  /* enable clipping for raw audio streams */
  s = gst_caps_get_structure (caps, 0);
  name = gst_structure_get_name (s);
  if (g_str_has_prefix (name, "audio/x-raw-")) {
    stream->need_clip = TRUE;
  }
  return caps;
}
