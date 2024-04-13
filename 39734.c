static s32 vmci_transport_error_to_vsock_error(s32 vmci_error)
{
	int err;

	switch (vmci_error) {
	case VMCI_ERROR_NO_MEM:
		err = ENOMEM;
		break;
	case VMCI_ERROR_DUPLICATE_ENTRY:
	case VMCI_ERROR_ALREADY_EXISTS:
		err = EADDRINUSE;
		break;
	case VMCI_ERROR_NO_ACCESS:
		err = EPERM;
		break;
	case VMCI_ERROR_NO_RESOURCES:
		err = ENOBUFS;
		break;
	case VMCI_ERROR_INVALID_RESOURCE:
		err = EHOSTUNREACH;
		break;
	case VMCI_ERROR_INVALID_ARGS:
	default:
		err = EINVAL;
	}

	return err > 0 ? -err : err;
}
