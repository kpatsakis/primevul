static long sock_prot_memory_allocated(struct proto *proto)
{
	return proto->memory_allocated != NULL ? proto_memory_allocated(proto) : -1L;
}
