static int ipc_memory_callback(struct notifier_block *self,
				unsigned long action, void *arg)
{
	static DECLARE_WORK(ipc_memory_wq, ipc_memory_notifier);

	switch (action) {
	case MEM_ONLINE:    /* memory successfully brought online */
	case MEM_OFFLINE:   /* or offline: it's time to recompute msgmni */
		/*
		 * This is done by invoking the ipcns notifier chain with the
		 * IPC_MEMCHANGED event.
		 * In order not to keep the lock on the hotplug memory chain
		 * for too long, queue a work item that will, when waken up,
		 * activate the ipcns notification chain.
		 * No need to keep several ipc work items on the queue.
		 */
		if (!work_pending(&ipc_memory_wq))
			schedule_work(&ipc_memory_wq);
		break;
	case MEM_GOING_ONLINE:
	case MEM_GOING_OFFLINE:
	case MEM_CANCEL_ONLINE:
	case MEM_CANCEL_OFFLINE:
	default:
		break;
	}

	return NOTIFY_OK;
}
