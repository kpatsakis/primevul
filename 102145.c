static OMX_U32 setPFramesSpacing(int32_t iFramesInterval, int32_t frameRate) {
 if (iFramesInterval < 0) {
 return 0xFFFFFFFF;
 } else if (iFramesInterval == 0) {
 return 0;
 }
    OMX_U32 ret = frameRate * iFramesInterval - 1;
 return ret;
}
