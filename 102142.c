status_t getOMXChannelMapping(size_t numChannels, OMX_AUDIO_CHANNELTYPE map[]) {
 switch (numChannels) {
 case 1:
 map[0] = OMX_AUDIO_ChannelCF;
 break;
 case 2:
 map[0] = OMX_AUDIO_ChannelLF;
 map[1] = OMX_AUDIO_ChannelRF;
 break;
 case 3:
 map[0] = OMX_AUDIO_ChannelLF;
 map[1] = OMX_AUDIO_ChannelRF;
 map[2] = OMX_AUDIO_ChannelCF;
 break;
 case 4:
 map[0] = OMX_AUDIO_ChannelLF;
 map[1] = OMX_AUDIO_ChannelRF;
 map[2] = OMX_AUDIO_ChannelLR;
 map[3] = OMX_AUDIO_ChannelRR;
 break;
 case 5:
 map[0] = OMX_AUDIO_ChannelLF;
 map[1] = OMX_AUDIO_ChannelRF;
 map[2] = OMX_AUDIO_ChannelCF;
 map[3] = OMX_AUDIO_ChannelLR;
 map[4] = OMX_AUDIO_ChannelRR;
 break;
 case 6:
 map[0] = OMX_AUDIO_ChannelLF;
 map[1] = OMX_AUDIO_ChannelRF;
 map[2] = OMX_AUDIO_ChannelCF;
 map[3] = OMX_AUDIO_ChannelLFE;
 map[4] = OMX_AUDIO_ChannelLR;
 map[5] = OMX_AUDIO_ChannelRR;
 break;
 case 7:
 map[0] = OMX_AUDIO_ChannelLF;
 map[1] = OMX_AUDIO_ChannelRF;
 map[2] = OMX_AUDIO_ChannelCF;
 map[3] = OMX_AUDIO_ChannelLFE;
 map[4] = OMX_AUDIO_ChannelLR;
 map[5] = OMX_AUDIO_ChannelRR;
 map[6] = OMX_AUDIO_ChannelCS;
 break;
 case 8:
 map[0] = OMX_AUDIO_ChannelLF;
 map[1] = OMX_AUDIO_ChannelRF;
 map[2] = OMX_AUDIO_ChannelCF;
 map[3] = OMX_AUDIO_ChannelLFE;
 map[4] = OMX_AUDIO_ChannelLR;
 map[5] = OMX_AUDIO_ChannelRR;
 map[6] = OMX_AUDIO_ChannelLS;
 map[7] = OMX_AUDIO_ChannelRS;
 break;
 default:
 return -EINVAL;
 }

 return OK;
}
