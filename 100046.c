rdp_out_ts_input_capabilityset(STREAM s)
{
	uint16 inputflags = 0;
	inputflags |= INPUT_FLAG_SCANCODES;

	out_uint16_le(s, RDP_CAPSET_INPUT);
	out_uint16_le(s, RDP_CAPLEN_INPUT);

	out_uint16_le(s, inputflags);	/* inputFlags */
	out_uint16_le(s, 0);	/* pad2OctetsA */
	out_uint32_le(s, 0x409);	/* keyboardLayout */
	out_uint32_le(s, 0x4);	/* keyboardType */
	out_uint32_le(s, 0);	/* keyboardSubtype */
	out_uint32_le(s, 0xC);	/* keyboardFunctionKey */
	out_utf16s_padded(s, "", 64, 0);	/* imeFileName */
}
