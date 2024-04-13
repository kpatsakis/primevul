GF_Box *tfrf_box_new()
{
	ISOM_DECL_BOX_ALLOC(GF_MSSTimeRefBox, GF_ISOM_BOX_TYPE_UUID);
	tmp->internal_4cc = GF_ISOM_BOX_UUID_TFRF;
	return (GF_Box *)tmp;
}