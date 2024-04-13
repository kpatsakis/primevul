dtls1_get_ccs_header(unsigned char *data, struct ccs_header_st *ccs_hdr)
	{
	memset(ccs_hdr, 0x00, sizeof(struct ccs_header_st));

	ccs_hdr->type = *(data++);
	}
