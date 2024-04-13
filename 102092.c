static const char *FourCC2MIME(uint32_t fourcc) {
 switch (fourcc) {
 case FOURCC('m', 'p', '4', 'a'):
 return MEDIA_MIMETYPE_AUDIO_AAC;

 case FOURCC('s', 'a', 'm', 'r'):
 return MEDIA_MIMETYPE_AUDIO_AMR_NB;

 case FOURCC('s', 'a', 'w', 'b'):
 return MEDIA_MIMETYPE_AUDIO_AMR_WB;

 case FOURCC('m', 'p', '4', 'v'):
 return MEDIA_MIMETYPE_VIDEO_MPEG4;

 case FOURCC('s', '2', '6', '3'):
 case FOURCC('h', '2', '6', '3'):
 case FOURCC('H', '2', '6', '3'):
 return MEDIA_MIMETYPE_VIDEO_H263;

 case FOURCC('a', 'v', 'c', '1'):
 return MEDIA_MIMETYPE_VIDEO_AVC;

 case FOURCC('h', 'v', 'c', '1'):
 case FOURCC('h', 'e', 'v', '1'):
 return MEDIA_MIMETYPE_VIDEO_HEVC;
 default:
            CHECK(!"should not be here.");
 return NULL;
 }
}
