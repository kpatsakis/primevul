status_t Camera2Client::storeMetaDataInBuffers(bool enabled) {
    ATRACE_CALL();
 Mutex::Autolock icl(mBinderSerializationLock);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 SharedParameters::Lock l(mParameters);
 switch (l.mParameters.state) {
 case Parameters::RECORD:
 case Parameters::VIDEO_SNAPSHOT:
            ALOGE("%s: Camera %d: Can't be called in state %s",
                    __FUNCTION__, mCameraId,
 Parameters::getStateName(l.mParameters.state));
 return INVALID_OPERATION;
 default:
 break;
 }

    l.mParameters.storeMetadataInBuffers = enabled;

 return OK;
}
