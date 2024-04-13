static void vmci_transport_qp_resumed_cb(u32 sub_id,
					 const struct vmci_event_data *e_data,
					 void *client_data)
{
	vsock_for_each_connected_socket(vmci_transport_handle_detach);
}
