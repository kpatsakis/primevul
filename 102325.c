bool SimpleSoftOMXComponent::isSetParameterAllowed(
        OMX_INDEXTYPE index, const OMX_PTR params) const {
 if (mState == OMX_StateLoaded) {
 return true;
 }

    OMX_U32 portIndex;

 switch (index) {
 case OMX_IndexParamPortDefinition:
 {
 const OMX_PARAM_PORTDEFINITIONTYPE *portDefs =
 (const OMX_PARAM_PORTDEFINITIONTYPE *) params;
 if (!isValidOMXParam(portDefs)) {
 return false;
 }
            portIndex = portDefs->nPortIndex;
 break;
 }

 case OMX_IndexParamAudioPcm:
 {
 const OMX_AUDIO_PARAM_PCMMODETYPE *pcmMode =
 (const OMX_AUDIO_PARAM_PCMMODETYPE *) params;
 if (!isValidOMXParam(pcmMode)) {
 return false;
 }
            portIndex = pcmMode->nPortIndex;
 break;
 }

 case OMX_IndexParamAudioAac:
 {
 const OMX_AUDIO_PARAM_AACPROFILETYPE *aacMode =
 (const OMX_AUDIO_PARAM_AACPROFILETYPE *) params;
 if (!isValidOMXParam(aacMode)) {
 return false;
 }
            portIndex = aacMode->nPortIndex;
 break;
 }

 default:
 return false;
 }

    CHECK(portIndex < mPorts.size());

 return !mPorts.itemAt(portIndex).mDef.bEnabled;
}
