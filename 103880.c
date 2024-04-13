static void ahci_commit_buf(IDEDMA *dma, uint32_t tx_bytes)
{
    AHCIDevice *ad = DO_UPCAST(AHCIDevice, dma, dma);

    tx_bytes += le32_to_cpu(ad->cur_cmd->status);
    ad->cur_cmd->status = cpu_to_le32(tx_bytes);
}
