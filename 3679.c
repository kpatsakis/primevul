
GF_Err dOps_box_write(GF_Box *s, GF_BitStream *bs)
{
	GF_Err e;
	GF_OpusSpecificBox *ptr = (GF_OpusSpecificBox *)s;
	if (!s) return GF_BAD_PARAM;
	e = gf_isom_box_write_header(s, bs);
	if (e) return e;
	gf_bs_write_u8(bs, ptr->version);
	gf_bs_write_u8(bs, ptr->OutputChannelCount);
	gf_bs_write_u16(bs, ptr->PreSkip);
	gf_bs_write_u32(bs, ptr->InputSampleRate);
	gf_bs_write_u16(bs, ptr->OutputGain);
	gf_bs_write_u8(bs, ptr->ChannelMappingFamily);
	if (ptr->ChannelMappingFamily) {
		gf_bs_write_u8(bs, ptr->StreamCount);
		gf_bs_write_u8(bs, ptr->CoupledCount);
		gf_bs_write_data(bs, (char *) ptr->ChannelMapping, ptr->OutputChannelCount);
	}
	return GF_OK;