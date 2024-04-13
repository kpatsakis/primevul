static int cirrus_bitblt_videotovideo_copy(CirrusVGAState * s)
{
    if (blit_is_unsafe(s, false))
        return 0;

    return cirrus_do_copy(s, s->cirrus_blt_dstaddr - s->vga.start_addr,
            s->cirrus_blt_srcaddr - s->vga.start_addr,
            s->cirrus_blt_width, s->cirrus_blt_height);
}
