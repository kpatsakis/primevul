static void pmcraid_handle_config_change(struct pmcraid_instance *pinstance)
{
	struct pmcraid_config_table_entry *cfg_entry;
	struct pmcraid_hcam_ccn *ccn_hcam;
	struct pmcraid_cmd *cmd;
	struct pmcraid_cmd *cfgcmd;
	struct pmcraid_resource_entry *res = NULL;
	unsigned long lock_flags;
	unsigned long host_lock_flags;
	u32 new_entry = 1;
	u32 hidden_entry = 0;
	u16 fw_version;
	int rc;

	ccn_hcam = (struct pmcraid_hcam_ccn *)pinstance->ccn.hcam;
	cfg_entry = &ccn_hcam->cfg_entry;
	fw_version = be16_to_cpu(pinstance->inq_data->fw_version);

	pmcraid_info("CCN(%x): %x timestamp: %llx type: %x lost: %x flags: %x \
		 res: %x:%x:%x:%x\n",
		 pinstance->ccn.hcam->ilid,
		 pinstance->ccn.hcam->op_code,
		((pinstance->ccn.hcam->timestamp1) |
		((pinstance->ccn.hcam->timestamp2 & 0xffffffffLL) << 32)),
		 pinstance->ccn.hcam->notification_type,
		 pinstance->ccn.hcam->notification_lost,
		 pinstance->ccn.hcam->flags,
		 pinstance->host->unique_id,
		 RES_IS_VSET(*cfg_entry) ? PMCRAID_VSET_BUS_ID :
		 (RES_IS_GSCSI(*cfg_entry) ? PMCRAID_PHYS_BUS_ID :
			RES_BUS(cfg_entry->resource_address)),
		 RES_IS_VSET(*cfg_entry) ?
			(fw_version <= PMCRAID_FW_VERSION_1 ?
				cfg_entry->unique_flags1 :
					cfg_entry->array_id & 0xFF) :
			RES_TARGET(cfg_entry->resource_address),
		 RES_LUN(cfg_entry->resource_address));


	/* If this HCAM indicates a lost notification, read the config table */
	if (pinstance->ccn.hcam->notification_lost) {
		cfgcmd = pmcraid_get_free_cmd(pinstance);
		if (cfgcmd) {
			pmcraid_info("lost CCN, reading config table\b");
			pinstance->reinit_cfg_table = 1;
			pmcraid_querycfg(cfgcmd);
		} else {
			pmcraid_err("lost CCN, no free cmd for querycfg\n");
		}
		goto out_notify_apps;
	}

	/* If this resource is not going to be added to mid-layer, just notify
	 * applications and return. If this notification is about hiding a VSET
	 * resource, check if it was exposed already.
	 */
	if (pinstance->ccn.hcam->notification_type ==
	    NOTIFICATION_TYPE_ENTRY_CHANGED &&
	    cfg_entry->resource_type == RES_TYPE_VSET) {

		if (fw_version <= PMCRAID_FW_VERSION_1)
			hidden_entry = (cfg_entry->unique_flags1 & 0x80) != 0;
		else
			hidden_entry = (cfg_entry->unique_flags1 & 0x80) != 0;

	} else if (!pmcraid_expose_resource(fw_version, cfg_entry)) {
		goto out_notify_apps;
	}

	spin_lock_irqsave(&pinstance->resource_lock, lock_flags);
	list_for_each_entry(res, &pinstance->used_res_q, queue) {
		rc = memcmp(&res->cfg_entry.resource_address,
			    &cfg_entry->resource_address,
			    sizeof(cfg_entry->resource_address));
		if (!rc) {
			new_entry = 0;
			break;
		}
	}

	if (new_entry) {

		if (hidden_entry) {
			spin_unlock_irqrestore(&pinstance->resource_lock,
						lock_flags);
			goto out_notify_apps;
		}

		/* If there are more number of resources than what driver can
		 * manage, do not notify the applications about the CCN. Just
		 * ignore this notifications and re-register the same HCAM
		 */
		if (list_empty(&pinstance->free_res_q)) {
			spin_unlock_irqrestore(&pinstance->resource_lock,
						lock_flags);
			pmcraid_err("too many resources attached\n");
			spin_lock_irqsave(pinstance->host->host_lock,
					  host_lock_flags);
			pmcraid_send_hcam(pinstance,
					  PMCRAID_HCAM_CODE_CONFIG_CHANGE);
			spin_unlock_irqrestore(pinstance->host->host_lock,
					       host_lock_flags);
			return;
		}

		res = list_entry(pinstance->free_res_q.next,
				 struct pmcraid_resource_entry, queue);

		list_del(&res->queue);
		res->scsi_dev = NULL;
		res->reset_progress = 0;
		list_add_tail(&res->queue, &pinstance->used_res_q);
	}

	memcpy(&res->cfg_entry, cfg_entry, pinstance->config_table_entry_size);

	if (pinstance->ccn.hcam->notification_type ==
	    NOTIFICATION_TYPE_ENTRY_DELETED || hidden_entry) {
		if (res->scsi_dev) {
			if (fw_version <= PMCRAID_FW_VERSION_1)
				res->cfg_entry.unique_flags1 &= 0x7F;
			else
				res->cfg_entry.array_id &= 0xFF;
			res->change_detected = RES_CHANGE_DEL;
			res->cfg_entry.resource_handle =
				PMCRAID_INVALID_RES_HANDLE;
			schedule_work(&pinstance->worker_q);
		} else {
			/* This may be one of the non-exposed resources */
			list_move_tail(&res->queue, &pinstance->free_res_q);
		}
	} else if (!res->scsi_dev) {
		res->change_detected = RES_CHANGE_ADD;
		schedule_work(&pinstance->worker_q);
	}
	spin_unlock_irqrestore(&pinstance->resource_lock, lock_flags);

out_notify_apps:

	/* Notify configuration changes to registered applications.*/
	if (!pmcraid_disable_aen)
		pmcraid_notify_ccn(pinstance);

	cmd = pmcraid_init_hcam(pinstance, PMCRAID_HCAM_CODE_CONFIG_CHANGE);
	if (cmd)
		pmcraid_send_hcam_cmd(cmd);
}
