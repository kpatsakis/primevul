GF_Err tfrf_box_write(GF_Box *s, GF_BitStream *bs)
{
	GF_Err e;
	u32 i;
	GF_MSSTimeRefBox *ptr = (GF_MSSTimeRefBox*)s;
	e = gf_isom_box_write_header(s, bs);
	if (e) return e;

	gf_bs_write_u8(bs, ptr->version);
	gf_bs_write_u24(bs, 0);
	gf_bs_write_u8(bs, ptr->frags_count);
	for (i=0; i<ptr->frags_count; i++) {
		if (ptr->version==0x01) {
			gf_bs_write_u64(bs, ptr->frags[i].absolute_time_in_track_timescale);
			gf_bs_write_u64(bs, ptr->frags[i].fragment_duration_in_track_timescale);
		} else {
			gf_bs_write_u32(bs, (u32) ptr->frags[i].absolute_time_in_track_timescale);
			gf_bs_write_u32(bs, (u32) ptr->frags[i].fragment_duration_in_track_timescale);
		}
	}
	return GF_OK;
}