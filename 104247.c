static int cirrus_bitblt_videotovideo(CirrusVGAState * s)
{
    int ret;

    if (s->cirrus_blt_mode & CIRRUS_BLTMODE_PATTERNCOPY) {
	ret = cirrus_bitblt_videotovideo_patterncopy(s);
    } else {
	ret = cirrus_bitblt_videotovideo_copy(s);
    }
    if (ret)
	cirrus_bitblt_reset(s);
    return ret;
}
