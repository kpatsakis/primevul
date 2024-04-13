void h264bsdFreeDpb(dpbStorage_t *dpb)
{

/* Variables */

    u32 i;

/* Code */

    ASSERT(dpb);

 if (dpb->buffer)
 {
 for (i = 0; i < dpb->dpbSize+1; i++)
 {
            FREE(dpb->buffer[i].pAllocatedData);
 }
 }
    FREE(dpb->buffer);
    FREE(dpb->list);
    FREE(dpb->outBuf);

}
