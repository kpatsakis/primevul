static int __init vmci_transport_init(void)
{
	int err;

	/* Create the datagram handle that we will use to send and receive all
	 * VSocket control messages for this context.
	 */
	err = vmci_transport_datagram_create_hnd(VMCI_TRANSPORT_PACKET_RID,
						 VMCI_FLAG_ANYCID_DG_HND,
						 vmci_transport_recv_stream_cb,
						 NULL,
						 &vmci_transport_stream_handle);
	if (err < VMCI_SUCCESS) {
		pr_err("Unable to create datagram handle. (%d)\n", err);
		return vmci_transport_error_to_vsock_error(err);
	}

	err = vmci_event_subscribe(VMCI_EVENT_QP_RESUMED,
				   vmci_transport_qp_resumed_cb,
				   NULL, &vmci_transport_qp_resumed_sub_id);
	if (err < VMCI_SUCCESS) {
		pr_err("Unable to subscribe to resumed event. (%d)\n", err);
		err = vmci_transport_error_to_vsock_error(err);
		vmci_transport_qp_resumed_sub_id = VMCI_INVALID_ID;
		goto err_destroy_stream_handle;
	}

	err = vsock_core_init(&vmci_transport);
	if (err < 0)
		goto err_unsubscribe;

	return 0;

err_unsubscribe:
	vmci_event_unsubscribe(vmci_transport_qp_resumed_sub_id);
err_destroy_stream_handle:
	vmci_datagram_destroy_handle(vmci_transport_stream_handle);
	return err;
}
