static bool vmci_transport_is_trusted(struct vsock_sock *vsock, u32 peer_cid)
{
	return vsock->trusted ||
	       vmci_is_context_owner(peer_cid, vsock->owner->uid);
}
