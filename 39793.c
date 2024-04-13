static int vmci_transport_socket_init(struct vsock_sock *vsk,
				      struct vsock_sock *psk)
{
	vsk->trans = kmalloc(sizeof(struct vmci_transport), GFP_KERNEL);
	if (!vsk->trans)
		return -ENOMEM;

	vmci_trans(vsk)->dg_handle = VMCI_INVALID_HANDLE;
	vmci_trans(vsk)->qp_handle = VMCI_INVALID_HANDLE;
	vmci_trans(vsk)->qpair = NULL;
	vmci_trans(vsk)->produce_size = vmci_trans(vsk)->consume_size = 0;
	vmci_trans(vsk)->attach_sub_id = vmci_trans(vsk)->detach_sub_id =
		VMCI_INVALID_ID;
	vmci_trans(vsk)->notify_ops = NULL;
	if (psk) {
		vmci_trans(vsk)->queue_pair_size =
			vmci_trans(psk)->queue_pair_size;
		vmci_trans(vsk)->queue_pair_min_size =
			vmci_trans(psk)->queue_pair_min_size;
		vmci_trans(vsk)->queue_pair_max_size =
			vmci_trans(psk)->queue_pair_max_size;
	} else {
		vmci_trans(vsk)->queue_pair_size =
			VMCI_TRANSPORT_DEFAULT_QP_SIZE;
		vmci_trans(vsk)->queue_pair_min_size =
			 VMCI_TRANSPORT_DEFAULT_QP_SIZE_MIN;
		vmci_trans(vsk)->queue_pair_max_size =
			VMCI_TRANSPORT_DEFAULT_QP_SIZE_MAX;
	}

	return 0;
}
