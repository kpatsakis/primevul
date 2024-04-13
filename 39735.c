static void __exit vmci_transport_exit(void)
{
	if (!vmci_handle_is_invalid(vmci_transport_stream_handle)) {
		if (vmci_datagram_destroy_handle(
			vmci_transport_stream_handle) != VMCI_SUCCESS)
			pr_err("Couldn't destroy datagram handle\n");
		vmci_transport_stream_handle = VMCI_INVALID_HANDLE;
	}

	if (vmci_transport_qp_resumed_sub_id != VMCI_INVALID_ID) {
		vmci_event_unsubscribe(vmci_transport_qp_resumed_sub_id);
		vmci_transport_qp_resumed_sub_id = VMCI_INVALID_ID;
	}

	vsock_core_exit();
}
