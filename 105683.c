static int ne2000_post_load(void* opaque, int version_id)
{
    NE2000State* s = opaque;

    if (version_id < 2) {
        s->rxcr = 0x0c;
    }
    return 0;
}
