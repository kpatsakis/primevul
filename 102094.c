char* MPEG4Extractor::getDrmTrackInfo(size_t trackID, int *len) {
 if (mFirstSINF == NULL) {
 return NULL;
 }

    SINF *sinf = mFirstSINF;
 while (sinf && (trackID != sinf->trackID)) {
        sinf = sinf->next;
 }

 if (sinf == NULL) {
 return NULL;
 }

 *len = sinf->len;
 return sinf->IPMPData;
}
