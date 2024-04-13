static int cirrus_vga_read_palette(CirrusVGAState * s)
{
    int val;

    if ((s->vga.sr[0x12] & CIRRUS_CURSOR_HIDDENPEL)) {
        val = s->cirrus_hidden_palette[(s->vga.dac_read_index & 0x0f) * 3 +
                                       s->vga.dac_sub_index];
    } else {
        val = s->vga.palette[s->vga.dac_read_index * 3 + s->vga.dac_sub_index];
    }
    if (++s->vga.dac_sub_index == 3) {
	s->vga.dac_sub_index = 0;
	s->vga.dac_read_index++;
    }
    return val;
}
