static void ahci_init_d2h(AHCIDevice *ad)
{
    uint8_t init_fis[20];
    IDEState *ide_state = &ad->port.ifs[0];

    memset(init_fis, 0, sizeof(init_fis));

    init_fis[4] = 1;
    init_fis[12] = 1;

    if (ide_state->drive_kind == IDE_CD) {
        init_fis[5] = ide_state->lcyl;
        init_fis[6] = ide_state->hcyl;
    }

    ahci_write_fis_d2h(ad, init_fis);
}
