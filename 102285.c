bool Camera2Client::recordingEnabled() {
    ATRACE_CALL();
 Mutex::Autolock icl(mBinderSerializationLock);

 if ( checkPid(__FUNCTION__) != OK) return false;

 return recordingEnabledL();
}
