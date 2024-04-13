GF_Err tfrf_box_read(GF_Box *s, GF_BitStream *bs)
{
	u32 i;
	GF_MSSTimeRefBox *ptr = (GF_MSSTimeRefBox *)s;
	ISOM_DECREASE_SIZE(ptr, 5);
	ptr->version = gf_bs_read_u8(bs);
	ptr->flags = gf_bs_read_u24(bs);
	ptr->frags_count = gf_bs_read_u8(bs);
	ptr->frags = gf_malloc(sizeof(GF_MSSTimeEntry) * ptr->frags_count);
	if (!ptr->frags) return GF_OUT_OF_MEM;

	for (i=0; i<ptr->frags_count; i++) {
		if (ptr->version == 0x01) {
			ISOM_DECREASE_SIZE(ptr, 16);
			ptr->frags[i].absolute_time_in_track_timescale = gf_bs_read_u64(bs);
			ptr->frags[i].fragment_duration_in_track_timescale = gf_bs_read_u64(bs);
		} else {
			ISOM_DECREASE_SIZE(ptr, 8);
			ptr->frags[i].absolute_time_in_track_timescale = gf_bs_read_u32(bs);
			ptr->frags[i].fragment_duration_in_track_timescale = gf_bs_read_u32(bs);
		}
	}
	return GF_OK;
}