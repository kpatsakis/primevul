void ACodec::LoadedState::onSetInputSurface(
 const sp<AMessage> &msg) {
    ALOGV("onSetInputSurface");

    sp<AMessage> notify = mCodec->mNotify->dup();
    notify->setInt32("what", CodecBase::kWhatInputSurfaceAccepted);

    sp<RefBase> obj;
    CHECK(msg->findObject("input-surface", &obj));
    sp<PersistentSurface> surface = static_cast<PersistentSurface *>(obj.get());

 status_t err = mCodec->mOMX->setInputSurface(
            mCodec->mNode, kPortIndexInput, surface->getBufferConsumer(),
 &mCodec->mInputMetadataType);

 if (err == OK) {
        err = setupInputSurface();
 }

 if (err != OK) {
        ALOGE("[%s] onSetInputSurface returning error %d",
                mCodec->mComponentName.c_str(), err);
        notify->setInt32("err", err);
 }
    notify->post();
}
