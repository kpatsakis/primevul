GF_Err mfra_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_MovieFragmentRandomAccessBox *ptr = (GF_MovieFragmentRandomAccessBox *)s;
	switch(a->type) {
	case GF_ISOM_BOX_TYPE_TFRA:
		BOX_FIELD_LIST_ASSIGN(tfra_list);
		return GF_OK;
	case GF_ISOM_BOX_TYPE_MFRO:
		BOX_FIELD_ASSIGN(mfro, GF_MovieFragmentRandomAccessOffsetBox)
		return GF_OK;
	}
	return GF_OK;
}