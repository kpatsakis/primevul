static void cirrus_cursor_invalidate(VGACommonState *s1)
{
    CirrusVGAState *s = container_of(s1, CirrusVGAState, vga);
    int size;

    if (!(s->vga.sr[0x12] & CIRRUS_CURSOR_SHOW)) {
        size = 0;
    } else {
        if (s->vga.sr[0x12] & CIRRUS_CURSOR_LARGE)
            size = 64;
        else
            size = 32;
    }
    /* invalidate last cursor and new cursor if any change */
    if (s->last_hw_cursor_size != size ||
        s->last_hw_cursor_x != s->hw_cursor_x ||
        s->last_hw_cursor_y != s->hw_cursor_y) {

        invalidate_cursor1(s);

        s->last_hw_cursor_size = size;
        s->last_hw_cursor_x = s->hw_cursor_x;
        s->last_hw_cursor_y = s->hw_cursor_y;
        /* compute the real cursor min and max y */
        cirrus_cursor_compute_yrange(s);
        invalidate_cursor1(s);
    }
}
