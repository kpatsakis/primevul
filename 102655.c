int sr_do_ioctl(Scsi_CD *cd, struct packet_command *cgc)
{
 	struct scsi_device *SDev;
 	struct scsi_sense_hdr sshdr;
 	int result, err = 0, retries = 0;
	unsigned char sense_buffer[SCSI_SENSE_BUFFERSIZE], *senseptr = NULL;
 
 	SDev = cd->device;
 
	if (cgc->sense)
		senseptr = sense_buffer;

       retry:
 	if (!scsi_block_when_processing_errors(SDev)) {
 		err = -ENODEV;
 		goto out;
 	}
 
 	result = scsi_execute(SDev, cgc->cmd, cgc->data_direction,
			      cgc->buffer, cgc->buflen, senseptr, &sshdr,
 			      cgc->timeout, IOCTL_RETRIES, 0, 0, NULL);
 
	if (cgc->sense)
		memcpy(cgc->sense, sense_buffer, sizeof(*cgc->sense));

 	/* Minimal error checking.  Ignore cases we know about, and report the rest. */
 	if (driver_byte(result) != 0) {
 		switch (sshdr.sense_key) {
		case UNIT_ATTENTION:
			SDev->changed = 1;
			if (!cgc->quiet)
				sr_printk(KERN_INFO, cd,
					  "disc change detected.\n");
			if (retries++ < 10)
				goto retry;
			err = -ENOMEDIUM;
			break;
		case NOT_READY:	/* This happens if there is no disc in drive */
			if (sshdr.asc == 0x04 &&
			    sshdr.ascq == 0x01) {
				/* sense: Logical unit is in process of becoming ready */
				if (!cgc->quiet)
					sr_printk(KERN_INFO, cd,
						  "CDROM not ready yet.\n");
				if (retries++ < 10) {
					/* sleep 2 sec and try again */
					ssleep(2);
					goto retry;
				} else {
					/* 20 secs are enough? */
					err = -ENOMEDIUM;
					break;
				}
			}
			if (!cgc->quiet)
				sr_printk(KERN_INFO, cd,
					  "CDROM not ready.  Make sure there "
					  "is a disc in the drive.\n");
			err = -ENOMEDIUM;
			break;
		case ILLEGAL_REQUEST:
			err = -EIO;
			if (sshdr.asc == 0x20 &&
			    sshdr.ascq == 0x00)
				/* sense: Invalid command operation code */
				err = -EDRIVE_CANT_DO_THIS;
			break;
		default:
			err = -EIO;
		}
	}

	/* Wake up a process waiting for device */
      out:
	cgc->stat = err;
	return err;
}
