static void cirrus_vga_write_palette(CirrusVGAState * s, int reg_value)
{
    s->vga.dac_cache[s->vga.dac_sub_index] = reg_value;
    if (++s->vga.dac_sub_index == 3) {
        if ((s->vga.sr[0x12] & CIRRUS_CURSOR_HIDDENPEL)) {
            memcpy(&s->cirrus_hidden_palette[(s->vga.dac_write_index & 0x0f) * 3],
                   s->vga.dac_cache, 3);
        } else {
            memcpy(&s->vga.palette[s->vga.dac_write_index * 3], s->vga.dac_cache, 3);
        }
        /* XXX update cursor */
	s->vga.dac_sub_index = 0;
	s->vga.dac_write_index++;
    }
}
