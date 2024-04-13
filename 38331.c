static int aac_queuecommand_lck(struct scsi_cmnd *cmd, void (*done)(struct scsi_cmnd *))
{
	struct Scsi_Host *host = cmd->device->host;
	struct aac_dev *dev = (struct aac_dev *)host->hostdata;
	u32 count = 0;
	cmd->scsi_done = done;
	for (; count < (host->can_queue + AAC_NUM_MGT_FIB); ++count) {
		struct fib * fib = &dev->fibs[count];
		struct scsi_cmnd * command;
		if (fib->hw_fib_va->header.XferState &&
		    ((command = fib->callback_data)) &&
		    (command == cmd) &&
		    (cmd->SCp.phase == AAC_OWNER_FIRMWARE))
			return 0; /* Already owned by Adapter */
	}
	cmd->SCp.phase = AAC_OWNER_LOWLEVEL;
	return (aac_scsi_cmd(cmd) ? FAILED : 0);
}
