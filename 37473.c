static int pmcraid_reset_bringup(struct pmcraid_instance *pinstance)
{
	pmcraid_notify_ioastate(pinstance, PMC_DEVICE_EVENT_RESET_START);

	return pmcraid_reset_reload(pinstance,
				    SHUTDOWN_NONE,
				    IOA_STATE_OPERATIONAL);
}
