static inline struct msg_queue *msg_lock(struct ipc_namespace *ns, int id)
{
	struct kern_ipc_perm *ipcp = ipc_lock(&msg_ids(ns), id);

	if (IS_ERR(ipcp))
		return (struct msg_queue *)ipcp;

	return container_of(ipcp, struct msg_queue, q_perm);
}
