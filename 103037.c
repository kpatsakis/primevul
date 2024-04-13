static int lsi_dma_ti64bit(LSIState *s)
{
    if ((s->ccntl1 & LSI_CCNTL1_EN64TIBMV) == LSI_CCNTL1_EN64TIBMV)
        return 1;
    return 0;
}
