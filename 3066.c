GF_Err tfxd_box_size(GF_Box *s)
{
	GF_MSSTimeExtBox *ptr = (GF_MSSTimeExtBox*)s;
	s->size += 4 + (ptr->version ? 16 : 8);
	return GF_OK;
}