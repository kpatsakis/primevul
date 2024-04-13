void ACodec::initiateShutdown(bool keepComponentAllocated) {
    sp<AMessage> msg = new AMessage(kWhatShutdown, this);
    msg->setInt32("keepComponentAllocated", keepComponentAllocated);
    msg->post();
 if (!keepComponentAllocated) {
 (new AMessage(kWhatReleaseCodecInstance, this))->post(3000000);
 }
}
