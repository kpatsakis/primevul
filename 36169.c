void emergency_restart(void)
{
	kmsg_dump(KMSG_DUMP_EMERG);
	machine_emergency_restart();
}
