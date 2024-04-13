static int unpack_v2KE(struct state *st,
		       struct pluto_crypto_req *r,
		       chunk_t *g)
{
	struct pcr_kenonce *kn = &r->pcr_d.kn;

	unpack_KE(st, r, g);
	return kn->oakley_group;
}
