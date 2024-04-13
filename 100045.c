rdp_out_ts_cache_definition(STREAM s, uint16 entries, uint16 maxcellsize)
{
	out_uint16_le(s, entries);
	out_uint16_le(s, maxcellsize);
}
