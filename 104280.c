static int cirrus_vga_read_gr(CirrusVGAState * s, unsigned reg_index)
{
    switch (reg_index) {
    case 0x00: // Standard VGA, BGCOLOR 0x000000ff
        return s->cirrus_shadow_gr0;
    case 0x01: // Standard VGA, FGCOLOR 0x000000ff
        return s->cirrus_shadow_gr1;
    case 0x02:			// Standard VGA
    case 0x03:			// Standard VGA
    case 0x04:			// Standard VGA
    case 0x06:			// Standard VGA
    case 0x07:			// Standard VGA
    case 0x08:			// Standard VGA
        return s->vga.gr[s->vga.gr_index];
    case 0x05:			// Standard VGA, Cirrus extended mode
    default:
	break;
    }

    if (reg_index < 0x3a) {
	return s->vga.gr[reg_index];
    } else {
#ifdef DEBUG_CIRRUS
	printf("cirrus: inport gr_index %02x\n", reg_index);
#endif
	return 0xff;
    }
}
