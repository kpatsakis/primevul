void h264bsdComputeSliceGroupMap(storage_t *pStorage, u32 sliceGroupChangeCycle)
{

/* Variables */


/* Code */

    h264bsdDecodeSliceGroupMap(pStorage->sliceGroupMap,
                        pStorage->activePps, sliceGroupChangeCycle,
                        pStorage->activeSps->picWidthInMbs,
                        pStorage->activeSps->picHeightInMbs);

}
