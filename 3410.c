GF_Err hnti_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_HintTrackInfoBox *ptr = (GF_HintTrackInfoBox *)s;
	if (!ptr || !a) return GF_BAD_PARAM;

	switch (a->type) {
	//this is the value for GF_RTPBox - same as HintSampleEntry for RTP !!!
	case GF_ISOM_BOX_TYPE_RTP:
	case GF_ISOM_BOX_TYPE_SDP:
		BOX_FIELD_ASSIGN(SDP, GF_Box)
		break;
	default:
		break;
	}
	return GF_OK;
}