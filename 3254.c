GF_Err mp4s_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_MPEGSampleEntryBox *ptr = (GF_MPEGSampleEntryBox *)s;
	switch (a->type) {
	case GF_ISOM_BOX_TYPE_ESDS:
		BOX_FIELD_ASSIGN(esd, GF_ESDBox)
		break;
	}
	return GF_OK;
}