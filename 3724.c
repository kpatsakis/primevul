GF_Err mvex_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_MovieExtendsBox *ptr = (GF_MovieExtendsBox *)s;

	switch (a->type) {
	case GF_ISOM_BOX_TYPE_TREX:
		BOX_FIELD_LIST_ASSIGN(TrackExList)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_TREP:
		BOX_FIELD_LIST_ASSIGN(TrackExPropList)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_MEHD:
		BOX_FIELD_ASSIGN(mehd, GF_MovieExtendsHeaderBox)
		return GF_OK;
	}
	return GF_OK;
}