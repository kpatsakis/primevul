static int pmcraid_notify_ldn(struct pmcraid_instance *pinstance)
{
	return pmcraid_notify_aen(pinstance,
				pinstance->ldn.msg,
				pinstance->ldn.hcam->data_len +
				sizeof(struct pmcraid_hcam_hdr));
}
