static int prdt_tbl_entry_size(const AHCI_SG *tbl)
{
    /* flags_size is zero-based */
    return (le32_to_cpu(tbl->flags_size) & AHCI_PRDT_SIZE_MASK) + 1;
}
