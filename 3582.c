GF_Err edts_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_EditBox *ptr = (GF_EditBox *)s;
	if (a->type == GF_ISOM_BOX_TYPE_ELST) {
		BOX_FIELD_ASSIGN(editList, GF_EditListBox)
		return GF_OK;
	} else {
		return GF_OK;
	}
	return GF_OK;
}