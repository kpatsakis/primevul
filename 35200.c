static int ape_read_close(AVFormatContext * s)
{
    APEContext *ape = s->priv_data;

    av_freep(&ape->frames);
    av_freep(&ape->seektable);
    return 0;
}
