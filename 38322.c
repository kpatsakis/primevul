static int aac_eh_abort(struct scsi_cmnd* cmd)
{
	struct scsi_device * dev = cmd->device;
	struct Scsi_Host * host = dev->host;
	struct aac_dev * aac = (struct aac_dev *)host->hostdata;
	int count;
	int ret = FAILED;

	printk(KERN_ERR "%s: Host adapter abort request (%d,%d,%d,%d)\n",
		AAC_DRIVERNAME,
		host->host_no, sdev_channel(dev), sdev_id(dev), dev->lun);
	switch (cmd->cmnd[0]) {
	case SERVICE_ACTION_IN:
		if (!(aac->raw_io_interface) ||
		    !(aac->raw_io_64) ||
		    ((cmd->cmnd[1] & 0x1f) != SAI_READ_CAPACITY_16))
			break;
	case INQUIRY:
	case READ_CAPACITY:
		/* Mark associated FIB to not complete, eh handler does this */
		for (count = 0; count < (host->can_queue + AAC_NUM_MGT_FIB); ++count) {
			struct fib * fib = &aac->fibs[count];
			if (fib->hw_fib_va->header.XferState &&
			  (fib->flags & FIB_CONTEXT_FLAG) &&
			  (fib->callback_data == cmd)) {
				fib->flags |= FIB_CONTEXT_FLAG_TIMED_OUT;
				cmd->SCp.phase = AAC_OWNER_ERROR_HANDLER;
				ret = SUCCESS;
			}
		}
		break;
	case TEST_UNIT_READY:
		/* Mark associated FIB to not complete, eh handler does this */
		for (count = 0; count < (host->can_queue + AAC_NUM_MGT_FIB); ++count) {
			struct scsi_cmnd * command;
			struct fib * fib = &aac->fibs[count];
			if ((fib->hw_fib_va->header.XferState & cpu_to_le32(Async | NoResponseExpected)) &&
			  (fib->flags & FIB_CONTEXT_FLAG) &&
			  ((command = fib->callback_data)) &&
			  (command->device == cmd->device)) {
				fib->flags |= FIB_CONTEXT_FLAG_TIMED_OUT;
				command->SCp.phase = AAC_OWNER_ERROR_HANDLER;
				if (command == cmd)
					ret = SUCCESS;
			}
		}
	}
	return ret;
}
