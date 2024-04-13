static int lsi_dma_40bit(LSIState *s)
{
    if ((s->ccntl1 & LSI_CCNTL1_40BIT) == LSI_CCNTL1_40BIT)
        return 1;
    return 0;
}
