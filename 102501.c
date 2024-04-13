static long pmcraid_ioctl_passthrough(
	struct pmcraid_instance *pinstance,
	unsigned int ioctl_cmd,
	unsigned int buflen,
	unsigned long arg
)
{
	struct pmcraid_passthrough_ioctl_buffer *buffer;
	struct pmcraid_ioarcb *ioarcb;
	struct pmcraid_cmd *cmd;
	struct pmcraid_cmd *cancel_cmd;
	unsigned long request_buffer;
	unsigned long request_offset;
	unsigned long lock_flags;
	void *ioasa;
	u32 ioasc;
	int request_size;
	int buffer_size;
	u8 access, direction;
	int rc = 0;

	/* If IOA reset is in progress, wait 10 secs for reset to complete */
	if (pinstance->ioa_reset_in_progress) {
		rc = wait_event_interruptible_timeout(
				pinstance->reset_wait_q,
				!pinstance->ioa_reset_in_progress,
				msecs_to_jiffies(10000));

		if (!rc)
			return -ETIMEDOUT;
		else if (rc < 0)
			return -ERESTARTSYS;
	}

	/* If adapter is not in operational state, return error */
	if (pinstance->ioa_state != IOA_STATE_OPERATIONAL) {
		pmcraid_err("IOA is not operational\n");
		return -ENOTTY;
	}

	buffer_size = sizeof(struct pmcraid_passthrough_ioctl_buffer);
	buffer = kmalloc(buffer_size, GFP_KERNEL);

	if (!buffer) {
		pmcraid_err("no memory for passthrough buffer\n");
		return -ENOMEM;
	}

	request_offset =
	    offsetof(struct pmcraid_passthrough_ioctl_buffer, request_buffer);

	request_buffer = arg + request_offset;

	rc = __copy_from_user(buffer,
			     (struct pmcraid_passthrough_ioctl_buffer *) arg,
			     sizeof(struct pmcraid_passthrough_ioctl_buffer));

	ioasa =
	(void *)(arg +
		offsetof(struct pmcraid_passthrough_ioctl_buffer, ioasa));

	if (rc) {
		pmcraid_err("ioctl: can't copy passthrough buffer\n");
		rc = -EFAULT;
		goto out_free_buffer;
	}

	request_size = buffer->ioarcb.data_transfer_length;

	if (buffer->ioarcb.request_flags0 & TRANSFER_DIR_WRITE) {
		access = VERIFY_READ;
		direction = DMA_TO_DEVICE;
	} else {
		access = VERIFY_WRITE;
		direction = DMA_FROM_DEVICE;
	}

	if (request_size > 0) {
		rc = access_ok(access, arg, request_offset + request_size);

		if (!rc) {
			rc = -EFAULT;
			goto out_free_buffer;
		}
	} else if (request_size < 0) {
		rc = -EINVAL;
		goto out_free_buffer;
	}

	/* check if we have any additional command parameters */
	if (buffer->ioarcb.add_cmd_param_length > PMCRAID_ADD_CMD_PARAM_LEN) {
		rc = -EINVAL;
		goto out_free_buffer;
	}

	cmd = pmcraid_get_free_cmd(pinstance);

	if (!cmd) {
		pmcraid_err("free command block is not available\n");
		rc = -ENOMEM;
		goto out_free_buffer;
	}

	cmd->scsi_cmd = NULL;
	ioarcb = &(cmd->ioa_cb->ioarcb);

	/* Copy the user-provided IOARCB stuff field by field */
	ioarcb->resource_handle = buffer->ioarcb.resource_handle;
	ioarcb->data_transfer_length = buffer->ioarcb.data_transfer_length;
	ioarcb->cmd_timeout = buffer->ioarcb.cmd_timeout;
	ioarcb->request_type = buffer->ioarcb.request_type;
	ioarcb->request_flags0 = buffer->ioarcb.request_flags0;
	ioarcb->request_flags1 = buffer->ioarcb.request_flags1;
	memcpy(ioarcb->cdb, buffer->ioarcb.cdb, PMCRAID_MAX_CDB_LEN);

	if (buffer->ioarcb.add_cmd_param_length) {
		ioarcb->add_cmd_param_length =
			buffer->ioarcb.add_cmd_param_length;
		ioarcb->add_cmd_param_offset =
			buffer->ioarcb.add_cmd_param_offset;
		memcpy(ioarcb->add_data.u.add_cmd_params,
			buffer->ioarcb.add_data.u.add_cmd_params,
			buffer->ioarcb.add_cmd_param_length);
	}

	/* set hrrq number where the IOA should respond to. Note that all cmds
	 * generated internally uses hrrq_id 0, exception to this is the cmd
	 * block of scsi_cmd which is re-used (e.g. cancel/abort), which uses
	 * hrrq_id assigned here in queuecommand
	 */
	ioarcb->hrrq_id = atomic_add_return(1, &(pinstance->last_message_id)) %
			  pinstance->num_hrrq;

	if (request_size) {
		rc = pmcraid_build_passthrough_ioadls(cmd,
						      request_size,
						      direction);
		if (rc) {
 			pmcraid_err("couldn't build passthrough ioadls\n");
 			goto out_free_buffer;
 		}
	} else if (request_size < 0) {
		rc = -EINVAL;
		goto out_free_buffer;
 	}
 
 	/* If data is being written into the device, copy the data from user
	 * buffers
	 */
	if (direction == DMA_TO_DEVICE && request_size > 0) {
		rc = pmcraid_copy_sglist(cmd->sglist,
					 request_buffer,
					 request_size,
					 direction);
		if (rc) {
			pmcraid_err("failed to copy user buffer\n");
			goto out_free_sglist;
		}
	}

	/* passthrough ioctl is a blocking command so, put the user to sleep
	 * until timeout. Note that a timeout value of 0 means, do timeout.
	 */
	cmd->cmd_done = pmcraid_internal_done;
	init_completion(&cmd->wait_for_completion);
	cmd->completion_req = 1;

	pmcraid_info("command(%d) (CDB[0] = %x) for %x\n",
		     le32_to_cpu(cmd->ioa_cb->ioarcb.response_handle) >> 2,
		     cmd->ioa_cb->ioarcb.cdb[0],
		     le32_to_cpu(cmd->ioa_cb->ioarcb.resource_handle));

	spin_lock_irqsave(pinstance->host->host_lock, lock_flags);
	_pmcraid_fire_command(cmd);
	spin_unlock_irqrestore(pinstance->host->host_lock, lock_flags);

	/* NOTE ! Remove the below line once abort_task is implemented
	 * in firmware. This line disables ioctl command timeout handling logic
	 * similar to IO command timeout handling, making ioctl commands to wait
	 * until the command completion regardless of timeout value specified in
	 * ioarcb
	 */
	buffer->ioarcb.cmd_timeout = 0;

	/* If command timeout is specified put caller to wait till that time,
	 * otherwise it would be blocking wait. If command gets timed out, it
	 * will be aborted.
	 */
	if (buffer->ioarcb.cmd_timeout == 0) {
		wait_for_completion(&cmd->wait_for_completion);
	} else if (!wait_for_completion_timeout(
			&cmd->wait_for_completion,
			msecs_to_jiffies(buffer->ioarcb.cmd_timeout * 1000))) {

		pmcraid_info("aborting cmd %d (CDB[0] = %x) due to timeout\n",
			le32_to_cpu(cmd->ioa_cb->ioarcb.response_handle >> 2),
			cmd->ioa_cb->ioarcb.cdb[0]);

		spin_lock_irqsave(pinstance->host->host_lock, lock_flags);
		cancel_cmd = pmcraid_abort_cmd(cmd);
		spin_unlock_irqrestore(pinstance->host->host_lock, lock_flags);

		if (cancel_cmd) {
			wait_for_completion(&cancel_cmd->wait_for_completion);
			ioasc = cancel_cmd->ioa_cb->ioasa.ioasc;
			pmcraid_return_cmd(cancel_cmd);

			/* if abort task couldn't find the command i.e it got
			 * completed prior to aborting, return good completion.
			 * if command got aborted successfully or there was IOA
			 * reset due to abort task itself getting timedout then
			 * return -ETIMEDOUT
			 */
			if (ioasc == PMCRAID_IOASC_IOA_WAS_RESET ||
			    PMCRAID_IOASC_SENSE_KEY(ioasc) == 0x00) {
				if (ioasc != PMCRAID_IOASC_GC_IOARCB_NOTFOUND)
					rc = -ETIMEDOUT;
				goto out_handle_response;
			}
		}

		/* no command block for abort task or abort task failed to abort
		 * the IOARCB, then wait for 150 more seconds and initiate reset
		 * sequence after timeout
		 */
		if (!wait_for_completion_timeout(
			&cmd->wait_for_completion,
			msecs_to_jiffies(150 * 1000))) {
			pmcraid_reset_bringup(cmd->drv_inst);
			rc = -ETIMEDOUT;
		}
	}

out_handle_response:
	/* copy entire IOASA buffer and return IOCTL success.
	 * If copying IOASA to user-buffer fails, return
	 * EFAULT
	 */
	if (copy_to_user(ioasa, &cmd->ioa_cb->ioasa,
		sizeof(struct pmcraid_ioasa))) {
		pmcraid_err("failed to copy ioasa buffer to user\n");
		rc = -EFAULT;
	}

	/* If the data transfer was from device, copy the data onto user
	 * buffers
	 */
	else if (direction == DMA_FROM_DEVICE && request_size > 0) {
		rc = pmcraid_copy_sglist(cmd->sglist,
					 request_buffer,
					 request_size,
					 direction);
		if (rc) {
			pmcraid_err("failed to copy user buffer\n");
			rc = -EFAULT;
		}
	}

out_free_sglist:
	pmcraid_release_passthrough_ioadls(cmd, request_size, direction);
	pmcraid_return_cmd(cmd);

out_free_buffer:
	kfree(buffer);

	return rc;
}
