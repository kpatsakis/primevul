static void pmcraid_set_timestamp(struct pmcraid_cmd *cmd)
{
	struct pmcraid_instance *pinstance = cmd->drv_inst;
	struct pmcraid_ioarcb *ioarcb = &cmd->ioa_cb->ioarcb;
	__be32 time_stamp_len = cpu_to_be32(PMCRAID_TIMESTAMP_LEN);
	struct pmcraid_ioadl_desc *ioadl = ioarcb->add_data.u.ioadl;

	struct timeval tv;
	__le64 timestamp;

	do_gettimeofday(&tv);
	timestamp = tv.tv_sec * 1000;

	pinstance->timestamp_data->timestamp[0] = (__u8)(timestamp);
	pinstance->timestamp_data->timestamp[1] = (__u8)((timestamp) >> 8);
	pinstance->timestamp_data->timestamp[2] = (__u8)((timestamp) >> 16);
	pinstance->timestamp_data->timestamp[3] = (__u8)((timestamp) >> 24);
	pinstance->timestamp_data->timestamp[4] = (__u8)((timestamp) >> 32);
	pinstance->timestamp_data->timestamp[5] = (__u8)((timestamp)  >> 40);

	pmcraid_reinit_cmdblk(cmd);
	ioarcb->request_type = REQ_TYPE_SCSI;
	ioarcb->resource_handle = cpu_to_le32(PMCRAID_IOA_RES_HANDLE);
	ioarcb->cdb[0] = PMCRAID_SCSI_SET_TIMESTAMP;
	ioarcb->cdb[1] = PMCRAID_SCSI_SERVICE_ACTION;
	memcpy(&(ioarcb->cdb[6]), &time_stamp_len, sizeof(time_stamp_len));

	ioarcb->ioadl_bus_addr = cpu_to_le64((cmd->ioa_cb_bus_addr) +
					offsetof(struct pmcraid_ioarcb,
						add_data.u.ioadl[0]));
	ioarcb->ioadl_length = cpu_to_le32(sizeof(struct pmcraid_ioadl_desc));
	ioarcb->ioarcb_bus_addr &= ~(0x1FULL);

	ioarcb->request_flags0 |= NO_LINK_DESCS;
	ioarcb->request_flags0 |= TRANSFER_DIR_WRITE;
	ioarcb->data_transfer_length =
		cpu_to_le32(sizeof(struct pmcraid_timestamp_data));
	ioadl = &(ioarcb->add_data.u.ioadl[0]);
	ioadl->flags = IOADL_FLAGS_LAST_DESC;
	ioadl->address = cpu_to_le64(pinstance->timestamp_data_baddr);
	ioadl->data_len = cpu_to_le32(sizeof(struct pmcraid_timestamp_data));

	if (!pinstance->timestamp_error) {
		pinstance->timestamp_error = 0;
		pmcraid_send_cmd(cmd, pmcraid_set_supported_devs,
			 PMCRAID_INTERNAL_TIMEOUT, pmcraid_timeout_handler);
	} else {
		pmcraid_send_cmd(cmd, pmcraid_return_cmd,
			 PMCRAID_INTERNAL_TIMEOUT, pmcraid_timeout_handler);
		return;
	}
}
