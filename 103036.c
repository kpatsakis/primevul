static int lsi_dma_64bit(LSIState *s)
{
    if ((s->ccntl1 & LSI_CCNTL1_EN64DBMV) == LSI_CCNTL1_EN64DBMV)
        return 1;
    return 0;
}
