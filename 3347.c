void tfrf_box_del(GF_Box *s)
{
	GF_MSSTimeRefBox *ptr = (GF_MSSTimeRefBox *)s;
	if (ptr->frags) gf_free(ptr->frags);
	gf_free(s);
}