vmci_transport_datagram_create_hnd(u32 resource_id,
				   u32 flags,
				   vmci_datagram_recv_cb recv_cb,
				   void *client_data,
				   struct vmci_handle *out_handle)
{
	int err = 0;

	/* Try to allocate our datagram handler as trusted. This will only work
	 * if vsock is running in the host.
	 */

	err = vmci_datagram_create_handle_priv(resource_id, flags,
					       VMCI_PRIVILEGE_FLAG_TRUSTED,
					       recv_cb,
					       client_data, out_handle);

	if (err == VMCI_ERROR_NO_ACCESS)
		err = vmci_datagram_create_handle(resource_id, flags,
						  recv_cb, client_data,
						  out_handle);

	return err;
}
