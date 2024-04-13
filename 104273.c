static void cirrus_update_memory_access(CirrusVGAState *s)
{
    unsigned mode;

    memory_region_transaction_begin();
    if ((s->vga.sr[0x17] & 0x44) == 0x44) {
        goto generic_io;
    } else if (s->cirrus_srcptr != s->cirrus_srcptr_end) {
        goto generic_io;
    } else {
	if ((s->vga.gr[0x0B] & 0x14) == 0x14) {
            goto generic_io;
	} else if (s->vga.gr[0x0B] & 0x02) {
            goto generic_io;
        }

	mode = s->vga.gr[0x05] & 0x7;
	if (mode < 4 || mode > 5 || ((s->vga.gr[0x0B] & 0x4) == 0)) {
            map_linear_vram(s);
        } else {
        generic_io:
            unmap_linear_vram(s);
        }
    }
    memory_region_transaction_commit();
}
