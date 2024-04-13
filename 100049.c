rdpsnd_send_waveconfirm(uint16 tick, uint8 packet_index)
{
	STREAM s;

	s = rdpsnd_init_packet(SNDC_WAVECONFIRM, 4);
	out_uint16_le(s, tick);
	out_uint8(s, packet_index);
	out_uint8(s, 0);
	s_mark_end(s);
	rdpsnd_send(s);

	logger(Sound, Debug, "rdpsnd_send_waveconfirm(), tick=%u, index=%u",
	       (unsigned) tick, (unsigned) packet_index);
}
