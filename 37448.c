static void pmcraid_notify_ioastate(struct pmcraid_instance *pinstance, u32 evt)
{
	pinstance->scn.ioa_state = evt;
	pmcraid_notify_aen(pinstance,
			  &pinstance->scn.msg,
			  sizeof(u32));
}
