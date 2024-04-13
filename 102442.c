bool ACodec::ExecutingToIdleState::onMessageReceived(const sp<AMessage> &msg) {
 bool handled = false;

 switch (msg->what()) {
 case kWhatFlush:
 {
            ALOGW("Ignoring flush request in ExecutingToIdleState");
 break;
 }

 case kWhatShutdown:
 {

            handled = true;
 break;
 }

 default:
            handled = BaseState::onMessageReceived(msg);
 break;
 }

 return handled;
}
