u32 OutputPicture(dpbStorage_t *dpb)
{

/* Variables */

 dpbPicture_t *tmp;

/* Code */

    ASSERT(dpb);

 if (dpb->noReordering)
 return(HANTRO_NOK);

    tmp = FindSmallestPicOrderCnt(dpb);

 /* no pictures to be displayed */
 if (tmp == NULL)
 return(HANTRO_NOK);

    dpb->outBuf[dpb->numOut].data  = tmp->data;
    dpb->outBuf[dpb->numOut].isIdr = tmp->isIdr;
    dpb->outBuf[dpb->numOut].picId = tmp->picId;
    dpb->outBuf[dpb->numOut].numErrMbs = tmp->numErrMbs;
    dpb->numOut++;

    tmp->toBeDisplayed = HANTRO_FALSE;
 if (!IS_REFERENCE(*tmp))
 {
        dpb->fullness--;
 }

 return(HANTRO_OK);

}
