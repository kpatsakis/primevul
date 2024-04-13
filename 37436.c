static void pmcraid_init_res_table(struct pmcraid_cmd *cmd)
{
	struct pmcraid_instance *pinstance = cmd->drv_inst;
	struct pmcraid_resource_entry *res, *temp;
	struct pmcraid_config_table_entry *cfgte;
	unsigned long lock_flags;
	int found, rc, i;
	u16 fw_version;
	LIST_HEAD(old_res);

	if (pinstance->cfg_table->flags & MICROCODE_UPDATE_REQUIRED)
		pmcraid_err("IOA requires microcode download\n");

	fw_version = be16_to_cpu(pinstance->inq_data->fw_version);

	/* resource list is protected by pinstance->resource_lock.
	 * init_res_table can be called from probe (user-thread) or runtime
	 * reset (timer/tasklet)
	 */
	spin_lock_irqsave(&pinstance->resource_lock, lock_flags);

	list_for_each_entry_safe(res, temp, &pinstance->used_res_q, queue)
		list_move_tail(&res->queue, &old_res);

	for (i = 0; i < pinstance->cfg_table->num_entries; i++) {
		if (be16_to_cpu(pinstance->inq_data->fw_version) <=
						PMCRAID_FW_VERSION_1)
			cfgte = &pinstance->cfg_table->entries[i];
		else
			cfgte = (struct pmcraid_config_table_entry *)
					&pinstance->cfg_table->entries_ext[i];

		if (!pmcraid_expose_resource(fw_version, cfgte))
			continue;

		found = 0;

		/* If this entry was already detected and initialized */
		list_for_each_entry_safe(res, temp, &old_res, queue) {

			rc = memcmp(&res->cfg_entry.resource_address,
				    &cfgte->resource_address,
				    sizeof(cfgte->resource_address));
			if (!rc) {
				list_move_tail(&res->queue,
						&pinstance->used_res_q);
				found = 1;
				break;
			}
		}

		/* If this is new entry, initialize it and add it the queue */
		if (!found) {

			if (list_empty(&pinstance->free_res_q)) {
				pmcraid_err("Too many devices attached\n");
				break;
			}

			found = 1;
			res = list_entry(pinstance->free_res_q.next,
					 struct pmcraid_resource_entry, queue);

			res->scsi_dev = NULL;
			res->change_detected = RES_CHANGE_ADD;
			res->reset_progress = 0;
			list_move_tail(&res->queue, &pinstance->used_res_q);
		}

		/* copy new configuration table entry details into driver
		 * maintained resource entry
		 */
		if (found) {
			memcpy(&res->cfg_entry, cfgte,
					pinstance->config_table_entry_size);
			pmcraid_info("New res type:%x, vset:%x, addr:%x:\n",
				 res->cfg_entry.resource_type,
				 (fw_version <= PMCRAID_FW_VERSION_1 ?
					res->cfg_entry.unique_flags1 :
						res->cfg_entry.array_id & 0xFF),
				 le32_to_cpu(res->cfg_entry.resource_address));
		}
	}

	/* Detect any deleted entries, mark them for deletion from mid-layer */
	list_for_each_entry_safe(res, temp, &old_res, queue) {

		if (res->scsi_dev) {
			res->change_detected = RES_CHANGE_DEL;
			res->cfg_entry.resource_handle =
				PMCRAID_INVALID_RES_HANDLE;
			list_move_tail(&res->queue, &pinstance->used_res_q);
		} else {
			list_move_tail(&res->queue, &pinstance->free_res_q);
		}
	}

	/* release the resource list lock */
	spin_unlock_irqrestore(&pinstance->resource_lock, lock_flags);
	pmcraid_set_timestamp(cmd);
}
