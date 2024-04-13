sp<MetaData> NuPlayer::GenericSource::getFormatMeta(bool audio) {
    sp<AMessage> msg = new AMessage(kWhatGetFormat, id());
    msg->setInt32("audio", audio);

    sp<AMessage> response;
 void *format;
 status_t err = msg->postAndAwaitResponse(&response);
 if (err == OK && response != NULL) {
        CHECK(response->findPointer("format", &format));
 return (MetaData *)format;
 } else {
 return NULL;
 }
}
