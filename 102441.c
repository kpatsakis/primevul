bool ACodec::LoadedState::onMessageReceived(const sp<AMessage> &msg) {
 bool handled = false;

 switch (msg->what()) {
 case ACodec::kWhatConfigureComponent:
 {
            onConfigureComponent(msg);
            handled = true;
 break;
 }

 case ACodec::kWhatCreateInputSurface:
 {
            onCreateInputSurface(msg);
            handled = true;
 break;
 }

 case ACodec::kWhatSetInputSurface:
 {
            onSetInputSurface(msg);
            handled = true;
 break;
 }

 case ACodec::kWhatStart:
 {
            onStart();
            handled = true;
 break;
 }

 case ACodec::kWhatShutdown:
 {
 int32_t keepComponentAllocated;
            CHECK(msg->findInt32(
 "keepComponentAllocated", &keepComponentAllocated));

            mCodec->mExplicitShutdown = true;
            onShutdown(keepComponentAllocated);

            handled = true;
 break;
 }

 case ACodec::kWhatFlush:
 {
            sp<AMessage> notify = mCodec->mNotify->dup();
            notify->setInt32("what", CodecBase::kWhatFlushCompleted);
            notify->post();

            handled = true;
 break;
 }

 default:
 return BaseState::onMessageReceived(msg);
 }

 return handled;
}
