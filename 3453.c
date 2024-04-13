GF_Err moof_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_MovieFragmentBox *ptr = (GF_MovieFragmentBox *)s;
	switch (a->type) {
	case GF_ISOM_BOX_TYPE_MFHD:
		BOX_FIELD_ASSIGN(mfhd, GF_MovieFragmentHeaderBox)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_TRAF:
		BOX_FIELD_LIST_ASSIGN(TrackList)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_PSSH:
		BOX_FIELD_LIST_ASSIGN(PSSHs)
		return GF_OK;
	}
	return GF_OK;
}