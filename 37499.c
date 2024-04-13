static void pmcraid_worker_function(struct work_struct *workp)
{
	struct pmcraid_instance *pinstance;
	struct pmcraid_resource_entry *res;
	struct pmcraid_resource_entry *temp;
	struct scsi_device *sdev;
	unsigned long lock_flags;
	unsigned long host_lock_flags;
	u16 fw_version;
	u8 bus, target, lun;

	pinstance = container_of(workp, struct pmcraid_instance, worker_q);
	/* add resources only after host is added into system */
	if (!atomic_read(&pinstance->expose_resources))
		return;

	fw_version = be16_to_cpu(pinstance->inq_data->fw_version);

	spin_lock_irqsave(&pinstance->resource_lock, lock_flags);
	list_for_each_entry_safe(res, temp, &pinstance->used_res_q, queue) {

		if (res->change_detected == RES_CHANGE_DEL && res->scsi_dev) {
			sdev = res->scsi_dev;

			/* host_lock must be held before calling
			 * scsi_device_get
			 */
			spin_lock_irqsave(pinstance->host->host_lock,
					  host_lock_flags);
			if (!scsi_device_get(sdev)) {
				spin_unlock_irqrestore(
						pinstance->host->host_lock,
						host_lock_flags);
				pmcraid_info("deleting %x from midlayer\n",
					     res->cfg_entry.resource_address);
				list_move_tail(&res->queue,
						&pinstance->free_res_q);
				spin_unlock_irqrestore(
					&pinstance->resource_lock,
					lock_flags);
				scsi_remove_device(sdev);
				scsi_device_put(sdev);
				spin_lock_irqsave(&pinstance->resource_lock,
						   lock_flags);
				res->change_detected = 0;
			} else {
				spin_unlock_irqrestore(
						pinstance->host->host_lock,
						host_lock_flags);
			}
		}
	}

	list_for_each_entry(res, &pinstance->used_res_q, queue) {

		if (res->change_detected == RES_CHANGE_ADD) {

			if (!pmcraid_expose_resource(fw_version,
						     &res->cfg_entry))
				continue;

			if (RES_IS_VSET(res->cfg_entry)) {
				bus = PMCRAID_VSET_BUS_ID;
				if (fw_version <= PMCRAID_FW_VERSION_1)
					target = res->cfg_entry.unique_flags1;
				else
					target = res->cfg_entry.array_id & 0xFF;
				lun = PMCRAID_VSET_LUN_ID;
			} else {
				bus = PMCRAID_PHYS_BUS_ID;
				target =
				     RES_TARGET(
					res->cfg_entry.resource_address);
				lun = RES_LUN(res->cfg_entry.resource_address);
			}

			res->change_detected = 0;
			spin_unlock_irqrestore(&pinstance->resource_lock,
						lock_flags);
			scsi_add_device(pinstance->host, bus, target, lun);
			spin_lock_irqsave(&pinstance->resource_lock,
					   lock_flags);
		}
	}

	spin_unlock_irqrestore(&pinstance->resource_lock, lock_flags);
}
