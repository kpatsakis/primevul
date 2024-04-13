static void aac_shutdown(struct pci_dev *dev)
{
	struct Scsi_Host *shost = pci_get_drvdata(dev);
	scsi_block_requests(shost);
	__aac_shutdown((struct aac_dev *)shost->hostdata);
}
