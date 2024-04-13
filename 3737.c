GF_Err dinf_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_DataInformationBox *ptr = (GF_DataInformationBox *)s;
	switch(a->type) {
	case GF_ISOM_BOX_TYPE_DREF:
		BOX_FIELD_ASSIGN(dref, GF_DataReferenceBox)
		return GF_OK;
	}
	return GF_OK;
}