static int aac_eh_reset(struct scsi_cmnd* cmd)
{
	struct scsi_device * dev = cmd->device;
	struct Scsi_Host * host = dev->host;
	struct scsi_cmnd * command;
	int count;
	struct aac_dev * aac = (struct aac_dev *)host->hostdata;
	unsigned long flags;

	/* Mark the associated FIB to not complete, eh handler does this */
	for (count = 0; count < (host->can_queue + AAC_NUM_MGT_FIB); ++count) {
		struct fib * fib = &aac->fibs[count];
		if (fib->hw_fib_va->header.XferState &&
		  (fib->flags & FIB_CONTEXT_FLAG) &&
		  (fib->callback_data == cmd)) {
			fib->flags |= FIB_CONTEXT_FLAG_TIMED_OUT;
			cmd->SCp.phase = AAC_OWNER_ERROR_HANDLER;
		}
	}
	printk(KERN_ERR "%s: Host adapter reset request. SCSI hang ?\n",
					AAC_DRIVERNAME);

	if ((count = aac_check_health(aac)))
		return count;
	/*
	 * Wait for all commands to complete to this specific
	 * target (block maximum 60 seconds).
	 */
	for (count = 60; count; --count) {
		int active = aac->in_reset;

		if (active == 0)
		__shost_for_each_device(dev, host) {
			spin_lock_irqsave(&dev->list_lock, flags);
			list_for_each_entry(command, &dev->cmd_list, list) {
				if ((command != cmd) &&
				    (command->SCp.phase == AAC_OWNER_FIRMWARE)) {
					active++;
					break;
				}
			}
			spin_unlock_irqrestore(&dev->list_lock, flags);
			if (active)
				break;

		}
		/*
		 * We can exit If all the commands are complete
		 */
		if (active == 0)
			return SUCCESS;
		ssleep(1);
	}
	printk(KERN_ERR "%s: SCSI bus appears hung\n", AAC_DRIVERNAME);
	/*
	 * This adapter needs a blind reset, only do so for Adapters that
	 * support a register, instead of a commanded, reset.
	 */
	if (((aac->supplement_adapter_info.SupportedOptions2 &
	  AAC_OPTION_MU_RESET) ||
	  (aac->supplement_adapter_info.SupportedOptions2 &
	  AAC_OPTION_DOORBELL_RESET)) &&
	  aac_check_reset &&
	  ((aac_check_reset != 1) ||
	   !(aac->supplement_adapter_info.SupportedOptions2 &
	    AAC_OPTION_IGNORE_RESET)))
		aac_reset_adapter(aac, 2); /* Bypass wait for command quiesce */
	return SUCCESS; /* Cause an immediate retry of the command with a ten second delay after successful tur */
}
