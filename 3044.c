GF_Err mdia_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_MediaBox *ptr = (GF_MediaBox *)s;
	switch(a->type) {
	case GF_ISOM_BOX_TYPE_MDHD:
		BOX_FIELD_ASSIGN(mediaHeader, GF_MediaHeaderBox)
		return GF_OK;

	case GF_ISOM_BOX_TYPE_HDLR:
		BOX_FIELD_ASSIGN(handler, GF_HandlerBox)
		return GF_OK;

	case GF_ISOM_BOX_TYPE_MINF:
		BOX_FIELD_ASSIGN(information, GF_MediaInformationBox)
		return GF_OK;
	}
	return GF_OK;
}