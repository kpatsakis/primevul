cifs_demultiplex_thread(void *p)
{
	int length;
	struct TCP_Server_Info *server = p;
	unsigned int pdu_length;
	char *buf = NULL;
	struct task_struct *task_to_wake = NULL;
	struct mid_q_entry *mid_entry;

	current->flags |= PF_MEMALLOC;
	cifs_dbg(FYI, "Demultiplex PID: %d\n", task_pid_nr(current));

	length = atomic_inc_return(&tcpSesAllocCount);
	if (length > 1)
		mempool_resize(cifs_req_poolp, length + cifs_min_rcv,
				GFP_KERNEL);

	set_freezable();
	while (server->tcpStatus != CifsExiting) {
		if (try_to_freeze())
			continue;

		if (!allocate_buffers(server))
			continue;

		server->large_buf = false;
		buf = server->smallbuf;
		pdu_length = 4; /* enough to get RFC1001 header */

		length = cifs_read_from_socket(server, buf, pdu_length);
		if (length < 0)
			continue;
		server->total_read = length;

		/*
		 * The right amount was read from socket - 4 bytes,
		 * so we can now interpret the length field.
		 */
		pdu_length = get_rfc1002_length(buf);

		cifs_dbg(FYI, "RFC1002 header 0x%x\n", pdu_length);
		if (!is_smb_response(server, buf[0]))
			continue;

		/* make sure we have enough to get to the MID */
		if (pdu_length < HEADER_SIZE(server) - 1 - 4) {
			cifs_dbg(VFS, "SMB response too short (%u bytes)\n",
				 pdu_length);
			cifs_reconnect(server);
			wake_up(&server->response_q);
			continue;
		}

		/* read down to the MID */
		length = cifs_read_from_socket(server, buf + 4,
					       HEADER_SIZE(server) - 1 - 4);
		if (length < 0)
			continue;
		server->total_read += length;

		mid_entry = server->ops->find_mid(server, buf);

		if (!mid_entry || !mid_entry->receive)
			length = standard_receive3(server, mid_entry);
		else
			length = mid_entry->receive(server, mid_entry);

		if (length < 0)
			continue;

		if (server->large_buf)
			buf = server->bigbuf;

		server->lstrp = jiffies;
		if (mid_entry != NULL) {
			if (!mid_entry->multiRsp || mid_entry->multiEnd)
				mid_entry->callback(mid_entry);
		} else if (!server->ops->is_oplock_break ||
			   !server->ops->is_oplock_break(buf, server)) {
			cifs_dbg(VFS, "No task to wake, unknown frame received! NumMids %d\n",
				 atomic_read(&midCount));
			cifs_dump_mem("Received Data is: ", buf,
				      HEADER_SIZE(server));
#ifdef CONFIG_CIFS_DEBUG2
			if (server->ops->dump_detail)
				server->ops->dump_detail(buf);
			cifs_dump_mids(server);
#endif /* CIFS_DEBUG2 */

		}
	} /* end while !EXITING */

	/* buffer usually freed in free_mid - need to free it here on exit */
	cifs_buf_release(server->bigbuf);
	if (server->smallbuf) /* no sense logging a debug message if NULL */
		cifs_small_buf_release(server->smallbuf);

	task_to_wake = xchg(&server->tsk, NULL);
	clean_demultiplex_info(server);

	/* if server->tsk was NULL then wait for a signal before exiting */
	if (!task_to_wake) {
		set_current_state(TASK_INTERRUPTIBLE);
		while (!signal_pending(current)) {
			schedule();
			set_current_state(TASK_INTERRUPTIBLE);
		}
		set_current_state(TASK_RUNNING);
	}

	module_put_and_exit(0);
}
