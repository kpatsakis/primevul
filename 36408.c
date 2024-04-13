power_pmu_notifier(struct notifier_block *self, unsigned long action, void *hcpu)
{
	unsigned int cpu = (long)hcpu;

	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_UP_PREPARE:
		power_pmu_setup(cpu);
		break;

	default:
		break;
	}

	return NOTIFY_OK;
}
