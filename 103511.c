qtdemux_video_caps (GstQTDemux * qtdemux, QtDemuxStream * stream,
    guint32 fourcc, const guint8 * stsd_data, gchar ** codec_name)
{
  GstCaps *caps;
  const GstStructure *s;
  const gchar *name;

  switch (fourcc) {
    case GST_MAKE_FOURCC ('p', 'n', 'g', ' '):
      _codec ("PNG still images");
      caps = gst_caps_new_simple ("image/png", NULL);
      break;
    case GST_MAKE_FOURCC ('j', 'p', 'e', 'g'):
      _codec ("JPEG still images");
      caps = gst_caps_new_simple ("image/jpeg", NULL);
      break;
    case GST_MAKE_FOURCC ('m', 'j', 'p', 'a'):
    case GST_MAKE_FOURCC ('A', 'V', 'D', 'J'):
    case GST_MAKE_FOURCC ('M', 'J', 'P', 'G'):
      _codec ("Motion-JPEG");
      caps = gst_caps_new_simple ("image/jpeg", NULL);
      break;
    case GST_MAKE_FOURCC ('m', 'j', 'p', 'b'):
      _codec ("Motion-JPEG format B");
      caps = gst_caps_new_simple ("video/x-mjpeg-b", NULL);
      break;
    case GST_MAKE_FOURCC ('m', 'j', 'p', '2'):
      _codec ("JPEG-2000");
      /* override to what it should be according to spec, avoid palette_data */
      stream->bits_per_sample = 24;
      caps = gst_caps_new_simple ("image/x-j2c", "fields", G_TYPE_INT, 1, NULL);
      break;
    case GST_MAKE_FOURCC ('S', 'V', 'Q', '3'):
      _codec ("Sorensen video v.3");
      caps = gst_caps_new_simple ("video/x-svq",
          "svqversion", G_TYPE_INT, 3, NULL);
      break;
    case GST_MAKE_FOURCC ('s', 'v', 'q', 'i'):
    case GST_MAKE_FOURCC ('S', 'V', 'Q', '1'):
      _codec ("Sorensen video v.1");
      caps = gst_caps_new_simple ("video/x-svq",
          "svqversion", G_TYPE_INT, 1, NULL);
      break;
    case GST_MAKE_FOURCC ('r', 'a', 'w', ' '):
    {
      guint16 bps;

      _codec ("Raw RGB video");
      bps = QT_UINT16 (stsd_data + 98);
      /* set common stuff */
      caps = gst_caps_new_simple ("video/x-raw-rgb",
          "endianness", G_TYPE_INT, G_BYTE_ORDER, "depth", G_TYPE_INT, bps,
          NULL);

      switch (bps) {
        case 15:
          gst_caps_set_simple (caps,
              "bpp", G_TYPE_INT, 16,
              "endianness", G_TYPE_INT, G_BIG_ENDIAN,
              "red_mask", G_TYPE_INT, 0x7c00,
              "green_mask", G_TYPE_INT, 0x03e0,
              "blue_mask", G_TYPE_INT, 0x001f, NULL);
          break;
        case 16:
          gst_caps_set_simple (caps,
              "bpp", G_TYPE_INT, 16,
              "endianness", G_TYPE_INT, G_BIG_ENDIAN,
              "red_mask", G_TYPE_INT, 0xf800,
              "green_mask", G_TYPE_INT, 0x07e0,
              "blue_mask", G_TYPE_INT, 0x001f, NULL);
          break;
        case 24:
          gst_caps_set_simple (caps,
              "bpp", G_TYPE_INT, 24,
              "endianness", G_TYPE_INT, G_BIG_ENDIAN,
              "red_mask", G_TYPE_INT, 0xff0000,
              "green_mask", G_TYPE_INT, 0x00ff00,
              "blue_mask", G_TYPE_INT, 0x0000ff, NULL);
          break;
        case 32:
          gst_caps_set_simple (caps,
              "bpp", G_TYPE_INT, 32,
              "endianness", G_TYPE_INT, G_BIG_ENDIAN,
              "alpha_mask", G_TYPE_INT, 0xff000000,
              "red_mask", G_TYPE_INT, 0x00ff0000,
              "green_mask", G_TYPE_INT, 0x0000ff00,
              "blue_mask", G_TYPE_INT, 0x000000ff, NULL);
          break;
        default:
          /* unknown */
          break;
      }
      break;
    }
    case GST_MAKE_FOURCC ('y', 'v', '1', '2'):
      _codec ("Raw planar YUV 4:2:0");
      caps = gst_caps_new_simple ("video/x-raw-yuv",
          "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('I', '4', '2', '0'),
          NULL);
      break;
    case GST_MAKE_FOURCC ('y', 'u', 'v', '2'):
    case GST_MAKE_FOURCC ('Y', 'u', 'v', '2'):
      _codec ("Raw packed YUV 4:2:2");
      caps = gst_caps_new_simple ("video/x-raw-yuv",
          "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('Y', 'U', 'Y', '2'),
          NULL);
      break;
    case GST_MAKE_FOURCC ('2', 'v', 'u', 'y'):
      _codec ("Raw packed YUV 4:2:0");
      caps = gst_caps_new_simple ("video/x-raw-yuv",
          "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC ('U', 'Y', 'V', 'Y'),
          NULL);
      break;
    case GST_MAKE_FOURCC ('m', 'p', 'e', 'g'):
    case GST_MAKE_FOURCC ('m', 'p', 'g', '1'):
      _codec ("MPEG-1 video");
      caps = gst_caps_new_simple ("video/mpeg", "mpegversion", G_TYPE_INT, 1,
          "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
      break;
    case GST_MAKE_FOURCC ('h', 'd', 'v', '1'): // HDV 720p30
    case GST_MAKE_FOURCC ('h', 'd', 'v', '2'): // HDV 1080i60
    case GST_MAKE_FOURCC ('h', 'd', 'v', '3'): // HDV 1080i50
    case GST_MAKE_FOURCC ('h', 'd', 'v', '5'): // HDV 720p25
    case GST_MAKE_FOURCC ('h', 'd', 'v', '6'): // HDV 1080i60
    case GST_MAKE_FOURCC ('m', 'x', '5', 'n'): // MPEG2 IMX NTSC 525/60 50mb/s produced by FCP
    case GST_MAKE_FOURCC ('m', 'x', '5', 'p'): // MPEG2 IMX PAL 625/60 50mb/s produced by FCP
    case GST_MAKE_FOURCC ('m', 'x', '4', 'n'): // MPEG2 IMX NTSC 525/60 40mb/s produced by FCP
    case GST_MAKE_FOURCC ('m', 'x', '4', 'p'): // MPEG2 IMX PAL 625/60 40mb/s produced by FCP
    case GST_MAKE_FOURCC ('m', 'x', '3', 'n'): // MPEG2 IMX NTSC 525/60 30mb/s produced by FCP
    case GST_MAKE_FOURCC ('m', 'x', '3', 'p'): // MPEG2 IMX PAL 625/50 30mb/s produced by FCP
    case GST_MAKE_FOURCC ('x', 'd', 'v', '2'): // XDCAM HD 1080i60
    case GST_MAKE_FOURCC ('A', 'V', 'm', 'p'): // AVID IMX PAL
    case GST_MAKE_FOURCC ('m', 'p', 'g', '2'): // AVID IMX PAL
      _codec ("MPEG-2 video");
      caps = gst_caps_new_simple ("video/mpeg", "mpegversion", G_TYPE_INT, 2,
          "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
      break;
    case GST_MAKE_FOURCC ('g', 'i', 'f', ' '):
      _codec ("GIF still images");
      caps = gst_caps_new_simple ("image/gif", NULL);
      break;
    case GST_MAKE_FOURCC ('h', '2', '6', '3'):
    case GST_MAKE_FOURCC ('H', '2', '6', '3'):
    case GST_MAKE_FOURCC ('s', '2', '6', '3'):
    case GST_MAKE_FOURCC ('U', '2', '6', '3'):
      _codec ("H.263");
      /* ffmpeg uses the height/width props, don't know why */
      caps = gst_caps_new_simple ("video/x-h263", NULL);
      break;
    case GST_MAKE_FOURCC ('m', 'p', '4', 'v'):
      _codec ("MPEG-4 video");
      caps = gst_caps_new_simple ("video/mpeg", "mpegversion", G_TYPE_INT, 4,
          "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
      break;
    case GST_MAKE_FOURCC ('3', 'i', 'v', 'd'):
    case GST_MAKE_FOURCC ('3', 'I', 'V', 'D'):
      _codec ("Microsoft MPEG-4 4.3");  /* FIXME? */
      caps = gst_caps_new_simple ("video/x-msmpeg",
          "msmpegversion", G_TYPE_INT, 43, NULL);
      break;
    case GST_MAKE_FOURCC ('3', 'I', 'V', '1'):
    case GST_MAKE_FOURCC ('3', 'I', 'V', '2'):
      _codec ("3ivX video");
      caps = gst_caps_new_simple ("video/x-3ivx", NULL);
      break;
    case GST_MAKE_FOURCC ('D', 'I', 'V', '3'):
      _codec ("DivX 3");
      caps = gst_caps_new_simple ("video/x-divx",
          "divxversion", G_TYPE_INT, 3, NULL);
      break;
    case GST_MAKE_FOURCC ('D', 'I', 'V', 'X'):
    case GST_MAKE_FOURCC ('d', 'i', 'v', 'x'):
      _codec ("DivX 4");
      caps = gst_caps_new_simple ("video/x-divx",
          "divxversion", G_TYPE_INT, 4, NULL);
      break;
    case GST_MAKE_FOURCC ('D', 'X', '5', '0'):
      _codec ("DivX 5");
      caps = gst_caps_new_simple ("video/x-divx",
          "divxversion", G_TYPE_INT, 5, NULL);
      break;
    case GST_MAKE_FOURCC ('X', 'V', 'I', 'D'):
    case GST_MAKE_FOURCC ('x', 'v', 'i', 'd'):
      _codec ("XVID MPEG-4");
      caps = gst_caps_new_simple ("video/x-xvid", NULL);
      break;

    case GST_MAKE_FOURCC ('F', 'M', 'P', '4'):
    case GST_MAKE_FOURCC ('U', 'M', 'P', '4'):
      caps = gst_caps_new_simple ("video/mpeg",
          "mpegversion", G_TYPE_INT, 4, NULL);
      if (codec_name)
        *codec_name = g_strdup ("FFmpeg MPEG-4");
      break;

    case GST_MAKE_FOURCC ('c', 'v', 'i', 'd'):
      _codec ("Cinepak");
      caps = gst_caps_new_simple ("video/x-cinepak", NULL);
      break;
    case GST_MAKE_FOURCC ('q', 'd', 'r', 'w'):
      _codec ("Apple QuickDraw");
      caps = gst_caps_new_simple ("video/x-qdrw", NULL);
      break;
    case GST_MAKE_FOURCC ('r', 'p', 'z', 'a'):
      _codec ("Apple video");
      caps = gst_caps_new_simple ("video/x-apple-video", NULL);
      break;
    case GST_MAKE_FOURCC ('a', 'v', 'c', '1'):
      _codec ("H.264 / AVC");
      caps = gst_caps_new_simple ("video/x-h264", NULL);
      break;
    case GST_MAKE_FOURCC ('r', 'l', 'e', ' '):
      _codec ("Run-length encoding");
      caps = gst_caps_new_simple ("video/x-rle",
          "layout", G_TYPE_STRING, "quicktime", NULL);
      break;
    case GST_MAKE_FOURCC ('i', 'v', '3', '2'):
      _codec ("Indeo Video 3");
      caps = gst_caps_new_simple ("video/x-indeo",
          "indeoversion", G_TYPE_INT, 3, NULL);
      break;
    case GST_MAKE_FOURCC ('I', 'V', '4', '1'):
    case GST_MAKE_FOURCC ('i', 'v', '4', '1'):
      _codec ("Intel Video 4");
      caps = gst_caps_new_simple ("video/x-indeo",
          "indeoversion", G_TYPE_INT, 4, NULL);
      break;
    case GST_MAKE_FOURCC ('d', 'v', 'c', 'p'):
    case GST_MAKE_FOURCC ('d', 'v', 'c', ' '):
    case GST_MAKE_FOURCC ('d', 'v', 's', 'd'):
    case GST_MAKE_FOURCC ('D', 'V', 'S', 'D'):
    case GST_MAKE_FOURCC ('d', 'v', 'c', 's'):
    case GST_MAKE_FOURCC ('D', 'V', 'C', 'S'):
    case GST_MAKE_FOURCC ('d', 'v', '2', '5'):
    case GST_MAKE_FOURCC ('d', 'v', 'p', 'p'):
      _codec ("DV Video");
      caps = gst_caps_new_simple ("video/x-dv", "dvversion", G_TYPE_INT, 25,
          "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
      break;
    case GST_MAKE_FOURCC ('d', 'v', '5', 'n'): //DVCPRO50 NTSC
    case GST_MAKE_FOURCC ('d', 'v', '5', 'p'): //DVCPRO50 PAL
      _codec ("DVCPro50 Video");
      caps = gst_caps_new_simple ("video/x-dv", "dvversion", G_TYPE_INT, 50,
          "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
      break;
    case GST_MAKE_FOURCC ('d', 'v', 'h', '5'): //DVCPRO HD 50i produced by FCP
    case GST_MAKE_FOURCC ('d', 'v', 'h', '6'): //DVCPRO HD 60i produced by FCP
      _codec ("DVCProHD Video");
      caps = gst_caps_new_simple ("video/x-dv", "dvversion", G_TYPE_INT, 100,
          "systemstream", G_TYPE_BOOLEAN, FALSE, NULL);
      break;
    case GST_MAKE_FOURCC ('s', 'm', 'c', ' '):
      _codec ("Apple Graphics (SMC)");
      caps = gst_caps_new_simple ("video/x-smc", NULL);
      break;
    case GST_MAKE_FOURCC ('V', 'P', '3', '1'):
      _codec ("VP3");
      caps = gst_caps_new_simple ("video/x-vp3", NULL);
      break;
    case GST_MAKE_FOURCC ('X', 'i', 'T', 'h'):
      _codec ("Theora");
      caps = gst_caps_new_simple ("video/x-theora", NULL);
      /* theora uses one byte of padding in the data stream because it does not
       * allow 0 sized packets while theora does */
      stream->padding = 1;
      break;
    case GST_MAKE_FOURCC ('d', 'r', 'a', 'c'):
      _codec ("Dirac");
      caps = gst_caps_new_simple ("video/x-dirac", NULL);
      break;
    case GST_MAKE_FOURCC ('t', 'i', 'f', 'f'):
      _codec ("TIFF still images");
      caps = gst_caps_new_simple ("image/tiff", NULL);
      break;
    case GST_MAKE_FOURCC ('k', 'p', 'c', 'd'):
    default:
    {
      char *s;

      s = g_strdup_printf ("video/x-gst-fourcc-%" GST_FOURCC_FORMAT,
          GST_FOURCC_ARGS (fourcc));
      caps = gst_caps_new_simple (s, NULL);
      break;
    }
  }

  /* enable clipping for raw video streams */
  s = gst_caps_get_structure (caps, 0);
  name = gst_structure_get_name (s);
  if (g_str_has_prefix (name, "video/x-raw-")) {
    stream->need_clip = TRUE;
  }
  return caps;
}
