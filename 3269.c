GF_Err tfxd_box_write(GF_Box *s, GF_BitStream *bs)
{
	GF_Err e;
	GF_MSSTimeExtBox *ptr = (GF_MSSTimeExtBox*)s;
	e = gf_isom_box_write_header(s, bs);
	if (e) return e;

	gf_bs_write_u8(bs, ptr->version);
	gf_bs_write_u24(bs, 0);
	if (ptr->version) {
		gf_bs_write_u64(bs, ptr->absolute_time_in_track_timescale);
		gf_bs_write_u64(bs, ptr->fragment_duration_in_track_timescale);
	} else {
		gf_bs_write_u32(bs, (u32) ptr->absolute_time_in_track_timescale);
		gf_bs_write_u32(bs, (u32) ptr->fragment_duration_in_track_timescale);
	}
	return GF_OK;
}