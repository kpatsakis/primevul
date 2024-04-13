static bool vmci_transport_dgram_allow(u32 cid, u32 port)
{
	if (cid == VMADDR_CID_HYPERVISOR) {
		/* Registrations of PBRPC Servers do not modify VMX/Hypervisor
		 * state and are allowed.
		 */
		return port == VMCI_UNITY_PBRPC_REGISTER;
	}

	return true;
}
