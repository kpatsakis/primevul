armpmu_release_hardware(struct arm_pmu *armpmu)
{
	armpmu->free_irq(armpmu);
	pm_runtime_put_sync(&armpmu->plat_device->dev);
}
