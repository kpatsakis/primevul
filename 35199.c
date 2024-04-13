static void ape_dumpinfo(AVFormatContext * s, APEContext * ape_ctx)
{
#if ENABLE_DEBUG
    int i;

    av_log(s, AV_LOG_DEBUG, "Descriptor Block:\n\n");
    av_log(s, AV_LOG_DEBUG, "magic                = \"%c%c%c%c\"\n", ape_ctx->magic[0], ape_ctx->magic[1], ape_ctx->magic[2], ape_ctx->magic[3]);
    av_log(s, AV_LOG_DEBUG, "fileversion          = %d\n", ape_ctx->fileversion);
    av_log(s, AV_LOG_DEBUG, "descriptorlength     = %d\n", ape_ctx->descriptorlength);
    av_log(s, AV_LOG_DEBUG, "headerlength         = %d\n", ape_ctx->headerlength);
    av_log(s, AV_LOG_DEBUG, "seektablelength      = %d\n", ape_ctx->seektablelength);
    av_log(s, AV_LOG_DEBUG, "wavheaderlength      = %d\n", ape_ctx->wavheaderlength);
    av_log(s, AV_LOG_DEBUG, "audiodatalength      = %d\n", ape_ctx->audiodatalength);
    av_log(s, AV_LOG_DEBUG, "audiodatalength_high = %d\n", ape_ctx->audiodatalength_high);
    av_log(s, AV_LOG_DEBUG, "wavtaillength        = %d\n", ape_ctx->wavtaillength);
    av_log(s, AV_LOG_DEBUG, "md5                  = ");
    for (i = 0; i < 16; i++)
         av_log(s, AV_LOG_DEBUG, "%02x", ape_ctx->md5[i]);
    av_log(s, AV_LOG_DEBUG, "\n");

    av_log(s, AV_LOG_DEBUG, "\nHeader Block:\n\n");

    av_log(s, AV_LOG_DEBUG, "compressiontype      = %d\n", ape_ctx->compressiontype);
    av_log(s, AV_LOG_DEBUG, "formatflags          = %d\n", ape_ctx->formatflags);
    av_log(s, AV_LOG_DEBUG, "blocksperframe       = %d\n", ape_ctx->blocksperframe);
    av_log(s, AV_LOG_DEBUG, "finalframeblocks     = %d\n", ape_ctx->finalframeblocks);
    av_log(s, AV_LOG_DEBUG, "totalframes          = %d\n", ape_ctx->totalframes);
    av_log(s, AV_LOG_DEBUG, "bps                  = %d\n", ape_ctx->bps);
    av_log(s, AV_LOG_DEBUG, "channels             = %d\n", ape_ctx->channels);
    av_log(s, AV_LOG_DEBUG, "samplerate           = %d\n", ape_ctx->samplerate);

    av_log(s, AV_LOG_DEBUG, "\nSeektable\n\n");
    if ((ape_ctx->seektablelength / sizeof(uint32_t)) != ape_ctx->totalframes) {
        av_log(s, AV_LOG_DEBUG, "No seektable\n");
    } else {
        for (i = 0; i < ape_ctx->seektablelength / sizeof(uint32_t); i++) {
            if (i < ape_ctx->totalframes - 1) {
                av_log(s, AV_LOG_DEBUG, "%8d   %d (%d bytes)\n", i, ape_ctx->seektable[i], ape_ctx->seektable[i + 1] - ape_ctx->seektable[i]);
            } else {
                av_log(s, AV_LOG_DEBUG, "%8d   %d\n", i, ape_ctx->seektable[i]);
            }
        }
    }

    av_log(s, AV_LOG_DEBUG, "\nFrames\n\n");
    for (i = 0; i < ape_ctx->totalframes; i++)
        av_log(s, AV_LOG_DEBUG, "%8d   %8lld %8d (%d samples)\n", i, ape_ctx->frames[i].pos, ape_ctx->frames[i].size, ape_ctx->frames[i].nblocks);

    av_log(s, AV_LOG_DEBUG, "\nCalculated information:\n\n");
    av_log(s, AV_LOG_DEBUG, "junklength           = %d\n", ape_ctx->junklength);
    av_log(s, AV_LOG_DEBUG, "firstframe           = %d\n", ape_ctx->firstframe);
    av_log(s, AV_LOG_DEBUG, "totalsamples         = %d\n", ape_ctx->totalsamples);
#endif
}
