static bool ship_v2KE(struct state *st,
		      struct pluto_crypto_req *r,
		      chunk_t *g,
		      pb_stream *outs, u_int8_t np)
{
	int oakley_group = unpack_v2KE(st, r, g);

	return justship_v2KE(st, g, oakley_group, outs, np);
}
