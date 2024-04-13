finish_realms()
{
    int i;

    for (i = 0; i < kdc_numrealms; i++) {
        finish_realm(kdc_realmlist[i]);
        kdc_realmlist[i] = 0;
    }
    kdc_numrealms = 0;
}
