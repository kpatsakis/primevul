OMX_ERRORTYPE SoftVPXEncoder::internalSetBitrateParams(
 const OMX_VIDEO_PARAM_BITRATETYPE* bitrate) {
 if (bitrate->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }

    mBitrate = bitrate->nTargetBitrate;

 if (bitrate->eControlRate == OMX_Video_ControlRateVariable) {
        mBitrateControlMode = VPX_VBR;
 } else if (bitrate->eControlRate == OMX_Video_ControlRateConstant) {
        mBitrateControlMode = VPX_CBR;
 } else {
 return OMX_ErrorUnsupportedSetting;
 }

 return OMX_ErrorNone;
}
