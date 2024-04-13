static void vmci_transport_destruct(struct vsock_sock *vsk)
{
	if (vmci_trans(vsk)->attach_sub_id != VMCI_INVALID_ID) {
		vmci_event_unsubscribe(vmci_trans(vsk)->attach_sub_id);
		vmci_trans(vsk)->attach_sub_id = VMCI_INVALID_ID;
	}

	if (vmci_trans(vsk)->detach_sub_id != VMCI_INVALID_ID) {
		vmci_event_unsubscribe(vmci_trans(vsk)->detach_sub_id);
		vmci_trans(vsk)->detach_sub_id = VMCI_INVALID_ID;
	}

	if (!vmci_handle_is_invalid(vmci_trans(vsk)->qp_handle)) {
		vmci_qpair_detach(&vmci_trans(vsk)->qpair);
		vmci_trans(vsk)->qp_handle = VMCI_INVALID_HANDLE;
		vmci_trans(vsk)->produce_size = 0;
		vmci_trans(vsk)->consume_size = 0;
	}

	if (vmci_trans(vsk)->notify_ops)
		vmci_trans(vsk)->notify_ops->socket_destruct(vsk);

	kfree(vsk->trans);
	vsk->trans = NULL;
}
