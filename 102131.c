void MediaPlayerService::Client::notify(
 void* cookie, int msg, int ext1, int ext2, const Parcel *obj)
{
 Client* client = static_cast<Client*>(cookie);
 if (client == NULL) {
 return;
 }

    sp<IMediaPlayerClient> c;
 {
 Mutex::Autolock l(client->mLock);
        c = client->mClient;
 if (msg == MEDIA_PLAYBACK_COMPLETE && client->mNextClient != NULL) {
 if (client->mAudioOutput != NULL)
                client->mAudioOutput->switchToNextOutput();
            client->mNextClient->start();
 if (client->mNextClient->mClient != NULL) {
                client->mNextClient->mClient->notify(
                        MEDIA_INFO, MEDIA_INFO_STARTED_AS_NEXT, 0, obj);
 }
 }
 }

 if (MEDIA_INFO == msg &&
        MEDIA_INFO_METADATA_UPDATE == ext1) {
 const media::Metadata::Type metadata_type = ext2;

 if(client->shouldDropMetadata(metadata_type)) {
 return;
 }

        client->addNewMetadataUpdate(metadata_type);
 }

 if (c != NULL) {
        ALOGV("[%d] notify (%p, %d, %d, %d)", client->mConnId, cookie, msg, ext1, ext2);
        c->notify(msg, ext1, ext2, obj);
 }
}
