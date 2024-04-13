void CameraService::BasicClient::disconnect() {
    ALOGV("BasicClient::disconnect");
    mCameraService->removeClientByRemote(mRemoteBinder);

     mClientPid = 0;
 }
