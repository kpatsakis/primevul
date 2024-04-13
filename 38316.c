static void __aac_shutdown(struct aac_dev * aac)
{
	if (aac->aif_thread) {
		int i;
		/* Clear out events first */
		for (i = 0; i < (aac->scsi_host_ptr->can_queue + AAC_NUM_MGT_FIB); i++) {
			struct fib *fib = &aac->fibs[i];
			if (!(fib->hw_fib_va->header.XferState & cpu_to_le32(NoResponseExpected | Async)) &&
			    (fib->hw_fib_va->header.XferState & cpu_to_le32(ResponseExpected)))
				up(&fib->event_wait);
		}
		kthread_stop(aac->thread);
	}
	aac_send_shutdown(aac);
	aac_adapter_disable_int(aac);
	free_irq(aac->pdev->irq, aac);
	if (aac->msi)
		pci_disable_msi(aac->pdev);
}
