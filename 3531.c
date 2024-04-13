GF_Err esds_box_size(GF_Box *s)
{
	u32 descSize = 0;
	GF_ESDBox *ptr = (GF_ESDBox *)s;
	//make sure we write with no ESID and no OCRESID
    if (ptr->desc) {
        ptr->desc->ESID = 0;
        ptr->desc->OCRESID = 0;
    }
	descSize = gf_odf_desc_size((GF_Descriptor *)ptr->desc);
	ptr->size += descSize;
	return GF_OK;
}