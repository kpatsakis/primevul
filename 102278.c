void CameraService::onDeviceStatusChanged(int cameraId,
 int newStatus)
{
    ALOGI("%s: Status changed for cameraId=%d, newStatus=%d", __FUNCTION__,
          cameraId, newStatus);

 if (cameraId < 0 || cameraId >= MAX_CAMERAS) {
        ALOGE("%s: Bad camera ID %d", __FUNCTION__, cameraId);
 return;
 }

 if ((int)getStatus(cameraId) == newStatus) {
        ALOGE("%s: State transition to the same status 0x%x not allowed",
              __FUNCTION__, (uint32_t)newStatus);
 return;
 }

 /* don't do this in updateStatus
       since it is also called from connect and we could get into a deadlock */
 if (newStatus == CAMERA_DEVICE_STATUS_NOT_PRESENT) {
 Vector<sp<BasicClient> > clientsToDisconnect;
 {
 Mutex::Autolock al(mServiceLock);

 /* Find all clients that we need to disconnect */
           sp<BasicClient> client = mClient[cameraId].promote();
 if (client.get() != NULL) {
               clientsToDisconnect.push_back(client);
 }

 int i = cameraId;
 for (size_t j = 0; j < mProClientList[i].size(); ++j) {
               sp<ProClient> cl = mProClientList[i][j].promote();
 if (cl != NULL) {
                   clientsToDisconnect.push_back(cl);
 }
 }
 }

 /* now disconnect them. don't hold the lock
           or we can get into a deadlock */

 for (size_t i = 0; i < clientsToDisconnect.size(); ++i) {
            sp<BasicClient> client = clientsToDisconnect[i];

            client->disconnect();
 /**
             * The remote app will no longer be able to call methods on the
             * client since the client PID will be reset to 0
             */
 }

        ALOGV("%s: After unplug, disconnected %d clients",
              __FUNCTION__, clientsToDisconnect.size());
 }

    updateStatus(
 static_cast<ICameraServiceListener::Status>(newStatus), cameraId);

}
