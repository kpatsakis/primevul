
GF_Err dOps_box_read(GF_Box *s, GF_BitStream *bs)
{
	GF_OpusSpecificBox *ptr = (GF_OpusSpecificBox *)s;
	ptr->version = gf_bs_read_u8(bs);
	ptr->OutputChannelCount = gf_bs_read_u8(bs);
	ptr->PreSkip = gf_bs_read_u16(bs);
	ptr->InputSampleRate = gf_bs_read_u32(bs);
	ptr->OutputGain = gf_bs_read_u16(bs);
	ptr->ChannelMappingFamily = gf_bs_read_u8(bs);
	ISOM_DECREASE_SIZE(ptr, 11)
	if (ptr->size) {
		ISOM_DECREASE_SIZE(ptr, 2+ptr->OutputChannelCount);
		ptr->StreamCount = gf_bs_read_u8(bs);
		ptr->CoupledCount = gf_bs_read_u8(bs);
		gf_bs_read_data(bs, (char *) ptr->ChannelMapping, ptr->OutputChannelCount);
	}
	return GF_OK;