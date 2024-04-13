static void cirrus_vga_write_sr(CirrusVGAState * s, uint32_t val)
{
    switch (s->vga.sr_index) {
    case 0x00:			// Standard VGA
    case 0x01:			// Standard VGA
    case 0x02:			// Standard VGA
    case 0x03:			// Standard VGA
    case 0x04:			// Standard VGA
	s->vga.sr[s->vga.sr_index] = val & sr_mask[s->vga.sr_index];
	if (s->vga.sr_index == 1)
            s->vga.update_retrace_info(&s->vga);
        break;
    case 0x06:			// Unlock Cirrus extensions
	val &= 0x17;
	if (val == 0x12) {
	    s->vga.sr[s->vga.sr_index] = 0x12;
	} else {
	    s->vga.sr[s->vga.sr_index] = 0x0f;
	}
	break;
    case 0x10:
    case 0x30:
    case 0x50:
    case 0x70:			// Graphics Cursor X
    case 0x90:
    case 0xb0:
    case 0xd0:
    case 0xf0:			// Graphics Cursor X
	s->vga.sr[0x10] = val;
	s->hw_cursor_x = (val << 3) | (s->vga.sr_index >> 5);
	break;
    case 0x11:
    case 0x31:
    case 0x51:
    case 0x71:			// Graphics Cursor Y
    case 0x91:
    case 0xb1:
    case 0xd1:
    case 0xf1:			// Graphics Cursor Y
	s->vga.sr[0x11] = val;
	s->hw_cursor_y = (val << 3) | (s->vga.sr_index >> 5);
	break;
    case 0x07:			// Extended Sequencer Mode
    cirrus_update_memory_access(s);
    case 0x08:			// EEPROM Control
    case 0x09:			// Scratch Register 0
    case 0x0a:			// Scratch Register 1
    case 0x0b:			// VCLK 0
    case 0x0c:			// VCLK 1
    case 0x0d:			// VCLK 2
    case 0x0e:			// VCLK 3
    case 0x0f:			// DRAM Control
    case 0x12:			// Graphics Cursor Attribute
    case 0x13:			// Graphics Cursor Pattern Address
    case 0x14:			// Scratch Register 2
    case 0x15:			// Scratch Register 3
    case 0x16:			// Performance Tuning Register
    case 0x18:			// Signature Generator Control
    case 0x19:			// Signature Generator Result
    case 0x1a:			// Signature Generator Result
    case 0x1b:			// VCLK 0 Denominator & Post
    case 0x1c:			// VCLK 1 Denominator & Post
    case 0x1d:			// VCLK 2 Denominator & Post
    case 0x1e:			// VCLK 3 Denominator & Post
    case 0x1f:			// BIOS Write Enable and MCLK select
	s->vga.sr[s->vga.sr_index] = val;
#ifdef DEBUG_CIRRUS
	printf("cirrus: handled outport sr_index %02x, sr_value %02x\n",
	       s->vga.sr_index, val);
#endif
	break;
    case 0x17:			// Configuration Readback and Extended Control
	s->vga.sr[s->vga.sr_index] = (s->vga.sr[s->vga.sr_index] & 0x38)
                                   | (val & 0xc7);
        cirrus_update_memory_access(s);
        break;
    default:
#ifdef DEBUG_CIRRUS
	printf("cirrus: outport sr_index %02x, sr_value %02x\n",
               s->vga.sr_index, val);
#endif
	break;
    }
}
