GF_Err tfrf_box_size(GF_Box *s)
{
	GF_MSSTimeRefBox *ptr = (GF_MSSTimeRefBox*)s;
	s->size += 5;
	if (ptr->version) s->size += 16 * ptr->frags_count;
	else s->size += 8 * ptr->frags_count;
	return GF_OK;
}