bool ACodec::IdleToLoadedState::onMessageReceived(const sp<AMessage> &msg) {
 bool handled = false;

 switch (msg->what()) {
 case kWhatShutdown:
 {

            handled = true;
 break;
 }

 case kWhatFlush:
 {
            ALOGE("Got flush request in IdleToLoadedState");
 break;
 }

 default:
            handled = BaseState::onMessageReceived(msg);
 break;
 }

 return handled;
}
