GF_Err srpp_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_SRTPProcessBox *ptr = (GF_SRTPProcessBox *)s;
	switch(a->type) {
	case GF_ISOM_BOX_TYPE_SCHI:
		BOX_FIELD_ASSIGN(info, GF_SchemeInformationBox)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_SCHM:
		BOX_FIELD_ASSIGN(scheme_type, GF_SchemeTypeBox)
		return GF_OK;
	}
	return GF_OK;
}