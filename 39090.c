SYSCALL_DEFINE2(msgget, key_t, key, int, msgflg)
{
	struct ipc_namespace *ns;
	struct ipc_ops msg_ops;
	struct ipc_params msg_params;

	ns = current->nsproxy->ipc_ns;

	msg_ops.getnew = newque;
	msg_ops.associate = msg_security;
	msg_ops.more_checks = NULL;

	msg_params.key = key;
	msg_params.flg = msgflg;

	return ipcget(ns, &msg_ids(ns), &msg_ops, &msg_params);
}
