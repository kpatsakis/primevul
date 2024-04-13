dpbOutPicture_t* h264bsdDpbOutputPicture(dpbStorage_t *dpb)
{

/* Variables */

/* Code */

    ASSERT(dpb);

 if (dpb->outIndex < dpb->numOut)
 return(dpb->outBuf + dpb->outIndex++);
 else
 return(NULL);

}
