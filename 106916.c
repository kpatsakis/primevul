static bool justship_v2KE(struct state *st UNUSED,
			  chunk_t *g, unsigned int oakley_group,
			  pb_stream *outs, u_int8_t np)
{
	struct ikev2_ke v2ke;
	pb_stream kepbs;

	memset(&v2ke, 0, sizeof(v2ke));
	v2ke.isak_np      = np;
	v2ke.isak_group   = oakley_group;
	if (!out_struct(&v2ke, &ikev2_ke_desc, outs, &kepbs))
		return FALSE;

	if (!out_chunk(*g, &kepbs, "ikev2 g^x"))
		return FALSE;

	close_output_pbs(&kepbs);
	return TRUE;
}
