status_t SoftAAC2::initDecoder() {
    ALOGV("initDecoder()");
 status_t status = UNKNOWN_ERROR;
    mAACDecoder = aacDecoder_Open(TT_MP4_ADIF, /* num layers */ 1);
 if (mAACDecoder != NULL) {
        mStreamInfo = aacDecoder_GetStreamInfo(mAACDecoder);
 if (mStreamInfo != NULL) {
            status = OK;
 }
 }

    mEndOfInput = false;
    mEndOfOutput = false;
    mOutputDelayCompensated = 0;
    mOutputDelayRingBufferSize = 2048 * MAX_CHANNEL_COUNT * kNumDelayBlocksMax;
    mOutputDelayRingBuffer = new short[mOutputDelayRingBufferSize];
    mOutputDelayRingBufferWritePos = 0;
    mOutputDelayRingBufferReadPos = 0;
    mOutputDelayRingBufferFilled = 0;

 if (mAACDecoder == NULL) {
        ALOGE("AAC decoder is null. TODO: Can not call aacDecoder_SetParam in the following code");
 }


    mDrcWrap.setDecoderHandle(mAACDecoder);
    mDrcWrap.submitStreamData(mStreamInfo);

 char value[PROPERTY_VALUE_MAX];
 if (property_get(PROP_DRC_OVERRIDE_REF_LEVEL, value, NULL)) {
 unsigned refLevel = atoi(value);
        ALOGV("AAC decoder using desired DRC target reference level of %d instead of %d", refLevel,
                DRC_DEFAULT_MOBILE_REF_LEVEL);
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_DESIRED_TARGET, refLevel);
 } else {
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_DESIRED_TARGET, DRC_DEFAULT_MOBILE_REF_LEVEL);
 }
 if (property_get(PROP_DRC_OVERRIDE_CUT, value, NULL)) {
 unsigned cut = atoi(value);
        ALOGV("AAC decoder using desired DRC attenuation factor of %d instead of %d", cut,
                DRC_DEFAULT_MOBILE_DRC_CUT);
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_DESIRED_ATT_FACTOR, cut);
 } else {
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_DESIRED_ATT_FACTOR, DRC_DEFAULT_MOBILE_DRC_CUT);
 }
 if (property_get(PROP_DRC_OVERRIDE_BOOST, value, NULL)) {
 unsigned boost = atoi(value);
        ALOGV("AAC decoder using desired DRC boost factor of %d instead of %d", boost,
                DRC_DEFAULT_MOBILE_DRC_BOOST);
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_DESIRED_BOOST_FACTOR, boost);
 } else {
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_DESIRED_BOOST_FACTOR, DRC_DEFAULT_MOBILE_DRC_BOOST);
 }
 if (property_get(PROP_DRC_OVERRIDE_HEAVY, value, NULL)) {
 unsigned heavy = atoi(value);
        ALOGV("AAC decoder using desried DRC heavy compression switch of %d instead of %d", heavy,
                DRC_DEFAULT_MOBILE_DRC_HEAVY);
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_DESIRED_HEAVY, heavy);
 } else {
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_DESIRED_HEAVY, DRC_DEFAULT_MOBILE_DRC_HEAVY);
 }
 if (property_get(PROP_DRC_OVERRIDE_ENC_LEVEL, value, NULL)) {
 unsigned encoderRefLevel = atoi(value);
        ALOGV("AAC decoder using encoder-side DRC reference level of %d instead of %d",
                encoderRefLevel, DRC_DEFAULT_MOBILE_ENC_LEVEL);
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_ENCODER_TARGET, encoderRefLevel);
 } else {
        mDrcWrap.setParam(DRC_PRES_MODE_WRAP_ENCODER_TARGET, DRC_DEFAULT_MOBILE_ENC_LEVEL);
 }

 return status;
}
