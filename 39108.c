static int msgctl_down(struct ipc_namespace *ns, int msqid, int cmd,
		       struct msqid_ds __user *buf, int version)
{
	struct kern_ipc_perm *ipcp;
	struct msqid64_ds uninitialized_var(msqid64);
	struct msg_queue *msq;
	int err;

	if (cmd == IPC_SET) {
		if (copy_msqid_from_user(&msqid64, buf, version))
			return -EFAULT;
	}

	ipcp = ipcctl_pre_down(ns, &msg_ids(ns), msqid, cmd,
			       &msqid64.msg_perm, msqid64.msg_qbytes);
	if (IS_ERR(ipcp))
		return PTR_ERR(ipcp);

	msq = container_of(ipcp, struct msg_queue, q_perm);

	err = security_msg_queue_msgctl(msq, cmd);
	if (err)
		goto out_unlock;

	switch (cmd) {
	case IPC_RMID:
		freeque(ns, ipcp);
		goto out_up;
	case IPC_SET:
		if (msqid64.msg_qbytes > ns->msg_ctlmnb &&
		    !capable(CAP_SYS_RESOURCE)) {
			err = -EPERM;
			goto out_unlock;
		}

		err = ipc_update_perm(&msqid64.msg_perm, ipcp);
		if (err)
			goto out_unlock;

		msq->q_qbytes = msqid64.msg_qbytes;

		msq->q_ctime = get_seconds();
		/* sleeping receivers might be excluded by
		 * stricter permissions.
		 */
		expunge_all(msq, -EAGAIN);
		/* sleeping senders might be able to send
		 * due to a larger queue size.
		 */
		ss_wakeup(&msq->q_senders, 0);
		break;
	default:
		err = -EINVAL;
	}
out_unlock:
	msg_unlock(msq);
out_up:
	up_write(&msg_ids(ns).rw_mutex);
	return err;
}
