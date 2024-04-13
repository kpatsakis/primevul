vmci_transport_queue_pair_alloc(struct vmci_qp **qpair,
				struct vmci_handle *handle,
				u64 produce_size,
				u64 consume_size,
				u32 peer, u32 flags, bool trusted)
{
	int err = 0;

	if (trusted) {
		/* Try to allocate our queue pair as trusted. This will only
		 * work if vsock is running in the host.
		 */

		err = vmci_qpair_alloc(qpair, handle, produce_size,
				       consume_size,
				       peer, flags,
				       VMCI_PRIVILEGE_FLAG_TRUSTED);
		if (err != VMCI_ERROR_NO_ACCESS)
			goto out;

	}

	err = vmci_qpair_alloc(qpair, handle, produce_size, consume_size,
			       peer, flags, VMCI_NO_PRIVILEGE_FLAGS);
out:
	if (err < 0) {
		pr_err("Could not attach to queue pair with %d\n",
		       err);
		err = vmci_transport_error_to_vsock_error(err);
	}

	return err;
}
