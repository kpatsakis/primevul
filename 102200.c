sp<MediaSource> ATSParser::Stream::getSource(SourceType type) {
 switch (type) {
 case VIDEO:
 {
 if (isVideo()) {
 return mSource;
 }
 break;
 }

 case AUDIO:
 {
 if (isAudio()) {
 return mSource;
 }
 break;
 }

 case META:
 {
 if (isMeta()) {
 return mSource;
 }
 break;
 }

 default:
 break;
 }

 return NULL;
}
