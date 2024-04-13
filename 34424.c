static av_cold int aac_parse_init(AVCodecParserContext *s1)
{
    AACAC3ParseContext *s = s1->priv_data;
    s->header_size = AAC_ADTS_HEADER_SIZE;
    s->sync = aac_sync;
    return 0;
}
