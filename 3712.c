GF_Err minf_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_MediaInformationBox *ptr = (GF_MediaInformationBox *)s;
	switch (a->type) {
	case GF_ISOM_BOX_TYPE_NMHD:
	case GF_ISOM_BOX_TYPE_STHD:
	case GF_ISOM_BOX_TYPE_VMHD:
	case GF_ISOM_BOX_TYPE_SMHD:
	case GF_ISOM_BOX_TYPE_HMHD:
	case GF_ISOM_BOX_TYPE_GMHD:
		BOX_FIELD_ASSIGN(InfoHeader, GF_Box)
		return GF_OK;

	case GF_ISOM_BOX_TYPE_DINF:
		BOX_FIELD_ASSIGN(dataInformation, GF_DataInformationBox)
		return GF_OK;

	case GF_ISOM_BOX_TYPE_STBL:
		BOX_FIELD_ASSIGN(sampleTable, GF_SampleTableBox)
		return GF_OK;
	}
	return GF_OK;
}