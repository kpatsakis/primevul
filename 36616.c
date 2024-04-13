nfs4_free_open_state(struct nfs4_state *state)
{
	kfree(state);
}
