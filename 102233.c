void Camera3Device::RequestThread::setNotificationListener(
 NotificationListener *listener) {
 Mutex::Autolock l(mRequestLock);
    mListener = listener;
}
