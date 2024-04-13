bool ATSParser::Stream::isAudio() const {
 switch (mStreamType) {
 case STREAMTYPE_MPEG1_AUDIO:
 case STREAMTYPE_MPEG2_AUDIO:
 case STREAMTYPE_MPEG2_AUDIO_ADTS:
 case STREAMTYPE_LPCM_AC3:
 case STREAMTYPE_AC3:
 return true;

 default:
 return false;
 }
}
