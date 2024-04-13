static void extractAlbumArt(
 const sp<MetaData> &fileMeta, const void *data, size_t size) {
    ALOGV("extractAlbumArt from '%s'", (const char *)data);

    sp<ABuffer> flacBuffer = decodeBase64(AString((const char *)data, size));
 if (flacBuffer == NULL) {
        ALOGE("malformed base64 encoded data.");
 return;
 }

 size_t flacSize = flacBuffer->size();
 uint8_t *flac = flacBuffer->data();
    ALOGV("got flac of size %zu", flacSize);

 uint32_t picType;
 uint32_t typeLen;
 uint32_t descLen;
 uint32_t dataLen;
 char type[128];

 if (flacSize < 8) {
 return;
 }

    picType = U32_AT(flac);

 if (picType != 3) {
 return;
 }

    typeLen = U32_AT(&flac[4]);
 if (typeLen > sizeof(type) - 1) {
 return;
 }

 if (flacSize - 8 < typeLen) {
 return;
 }

    memcpy(type, &flac[8], typeLen);
    type[typeLen] = '\0';

    ALOGV("picType = %d, type = '%s'", picType, type);

 if (!strcmp(type, "-->")) {
 return;
 }

 if (flacSize < 32 || flacSize - 32 < typeLen) {
 return;
 }

    descLen = U32_AT(&flac[8 + typeLen]);
 if (flacSize - 32 - typeLen < descLen) {
 return;
 }

    dataLen = U32_AT(&flac[8 + typeLen + 4 + descLen + 16]);

 if (flacSize - 32 - typeLen - descLen < dataLen) {
 return;
 }

    ALOGV("got image data, %zu trailing bytes",
         flacSize - 32 - typeLen - descLen - dataLen);

    fileMeta->setData(
            kKeyAlbumArt, 0, &flac[8 + typeLen + 4 + descLen + 20], dataLen);

    fileMeta->setCString(kKeyAlbumArtMIME, type);
}
