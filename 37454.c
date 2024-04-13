static void pmcraid_querycfg(struct pmcraid_cmd *cmd)
{
	struct pmcraid_ioarcb *ioarcb = &cmd->ioa_cb->ioarcb;
	struct pmcraid_ioadl_desc *ioadl = ioarcb->add_data.u.ioadl;
	struct pmcraid_instance *pinstance = cmd->drv_inst;
	int cfg_table_size = cpu_to_be32(sizeof(struct pmcraid_config_table));

	if (be16_to_cpu(pinstance->inq_data->fw_version) <=
					PMCRAID_FW_VERSION_1)
		pinstance->config_table_entry_size =
			sizeof(struct pmcraid_config_table_entry);
	else
		pinstance->config_table_entry_size =
			sizeof(struct pmcraid_config_table_entry_ext);

	ioarcb->request_type = REQ_TYPE_IOACMD;
	ioarcb->resource_handle = cpu_to_le32(PMCRAID_IOA_RES_HANDLE);

	ioarcb->cdb[0] = PMCRAID_QUERY_IOA_CONFIG;

	/* firmware requires 4-byte length field, specified in B.E format */
	memcpy(&(ioarcb->cdb[10]), &cfg_table_size, sizeof(cfg_table_size));

	/* Since entire config table can be described by single IOADL, it can
	 * be part of IOARCB itself
	 */
	ioarcb->ioadl_bus_addr = cpu_to_le64((cmd->ioa_cb_bus_addr) +
					offsetof(struct pmcraid_ioarcb,
						add_data.u.ioadl[0]));
	ioarcb->ioadl_length = cpu_to_le32(sizeof(struct pmcraid_ioadl_desc));
	ioarcb->ioarcb_bus_addr &= ~(0x1FULL);

	ioarcb->request_flags0 |= NO_LINK_DESCS;
	ioarcb->data_transfer_length =
		cpu_to_le32(sizeof(struct pmcraid_config_table));

	ioadl = &(ioarcb->add_data.u.ioadl[0]);
	ioadl->flags = IOADL_FLAGS_LAST_DESC;
	ioadl->address = cpu_to_le64(pinstance->cfg_table_bus_addr);
	ioadl->data_len = cpu_to_le32(sizeof(struct pmcraid_config_table));

	pmcraid_send_cmd(cmd, pmcraid_init_res_table,
			 PMCRAID_INTERNAL_TIMEOUT, pmcraid_timeout_handler);
}
