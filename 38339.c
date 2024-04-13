static int aac_slave_configure(struct scsi_device *sdev)
{
	struct aac_dev *aac = (struct aac_dev *)sdev->host->hostdata;
	if (aac->jbod && (sdev->type == TYPE_DISK))
		sdev->removable = 1;
	if ((sdev->type == TYPE_DISK) &&
			(sdev_channel(sdev) != CONTAINER_CHANNEL) &&
			(!aac->jbod || sdev->inq_periph_qual) &&
			(!aac->raid_scsi_mode || (sdev_channel(sdev) != 2))) {
		if (expose_physicals == 0)
			return -ENXIO;
		if (expose_physicals < 0)
			sdev->no_uld_attach = 1;
	}
	if (sdev->tagged_supported && (sdev->type == TYPE_DISK) &&
			(!aac->raid_scsi_mode || (sdev_channel(sdev) != 2)) &&
			!sdev->no_uld_attach) {
		struct scsi_device * dev;
		struct Scsi_Host *host = sdev->host;
		unsigned num_lsu = 0;
		unsigned num_one = 0;
		unsigned depth;
		unsigned cid;

		/*
		 * Firmware has an individual device recovery time typically
		 * of 35 seconds, give us a margin.
		 */
		if (sdev->request_queue->rq_timeout < (45 * HZ))
			blk_queue_rq_timeout(sdev->request_queue, 45*HZ);
		for (cid = 0; cid < aac->maximum_num_containers; ++cid)
			if (aac->fsa_dev[cid].valid)
				++num_lsu;
		__shost_for_each_device(dev, host) {
			if (dev->tagged_supported && (dev->type == TYPE_DISK) &&
					(!aac->raid_scsi_mode ||
						(sdev_channel(sdev) != 2)) &&
					!dev->no_uld_attach) {
				if ((sdev_channel(dev) != CONTAINER_CHANNEL)
				 || !aac->fsa_dev[sdev_id(dev)].valid)
					++num_lsu;
			} else
				++num_one;
		}
		if (num_lsu == 0)
			++num_lsu;
		depth = (host->can_queue - num_one) / num_lsu;
		if (depth > 256)
			depth = 256;
		else if (depth < 2)
			depth = 2;
		scsi_adjust_queue_depth(sdev, MSG_ORDERED_TAG, depth);
	} else
		scsi_adjust_queue_depth(sdev, 0, 1);

	return 0;
}
