void SoftAAC2::onPortEnableCompleted(OMX_U32 portIndex, bool enabled) {
 if (portIndex != 1) {
 return;
 }

 switch (mOutputPortSettingsChange) {
 case NONE:
 break;

 case AWAITING_DISABLED:
 {
            CHECK(!enabled);
            mOutputPortSettingsChange = AWAITING_ENABLED;
 break;
 }

 default:
 {
            CHECK_EQ((int)mOutputPortSettingsChange, (int)AWAITING_ENABLED);
            CHECK(enabled);
            mOutputPortSettingsChange = NONE;
 break;
 }
 }
}
