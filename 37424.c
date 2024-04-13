static void pmcraid_get_fwversion(struct pmcraid_cmd *cmd)
{
	struct pmcraid_ioarcb *ioarcb = &cmd->ioa_cb->ioarcb;
	struct pmcraid_ioadl_desc *ioadl = ioarcb->add_data.u.ioadl;
	struct pmcraid_instance *pinstance = cmd->drv_inst;
	u16 data_size = sizeof(struct pmcraid_inquiry_data);

	pmcraid_reinit_cmdblk(cmd);
	ioarcb->request_type = REQ_TYPE_SCSI;
	ioarcb->resource_handle = cpu_to_le32(PMCRAID_IOA_RES_HANDLE);
	ioarcb->cdb[0] = INQUIRY;
	ioarcb->cdb[1] = 1;
	ioarcb->cdb[2] = 0xD0;
	ioarcb->cdb[3] = (data_size >> 8) & 0xFF;
	ioarcb->cdb[4] = data_size & 0xFF;

	/* Since entire inquiry data it can be part of IOARCB itself
	 */
	ioarcb->ioadl_bus_addr = cpu_to_le64((cmd->ioa_cb_bus_addr) +
					offsetof(struct pmcraid_ioarcb,
						add_data.u.ioadl[0]));
	ioarcb->ioadl_length = cpu_to_le32(sizeof(struct pmcraid_ioadl_desc));
	ioarcb->ioarcb_bus_addr &= ~(0x1FULL);

	ioarcb->request_flags0 |= NO_LINK_DESCS;
	ioarcb->data_transfer_length = cpu_to_le32(data_size);
	ioadl = &(ioarcb->add_data.u.ioadl[0]);
	ioadl->flags = IOADL_FLAGS_LAST_DESC;
	ioadl->address = cpu_to_le64(pinstance->inq_data_baddr);
	ioadl->data_len = cpu_to_le32(data_size);

	pmcraid_send_cmd(cmd, pmcraid_get_fwversion_done,
			 PMCRAID_INTERNAL_TIMEOUT, pmcraid_timeout_handler);
}
