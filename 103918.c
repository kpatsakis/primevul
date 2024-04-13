static int is_ncq(uint8_t ata_cmd)
{
    /* Based on SATA 3.2 section 13.6.3.2 */
    switch (ata_cmd) {
    case READ_FPDMA_QUEUED:
    case WRITE_FPDMA_QUEUED:
    case NCQ_NON_DATA:
    case RECEIVE_FPDMA_QUEUED:
    case SEND_FPDMA_QUEUED:
        return 1;
    default:
        return 0;
    }
}
