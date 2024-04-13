static GF_Err FFD_ConnectChannel(GF_InputService *plug, LPNETCHANNEL channel, const char *url, Bool upstream)
{
	GF_Err e;
	u32 ESID;
	FFDemux *ffd = (FFDemux*)plug->priv;

	e = GF_STREAM_NOT_FOUND;
	if (upstream) {
		e = GF_ISOM_INVALID_FILE;
		goto exit;
	}
	if (!strstr(url, "ES_ID=")) {
		e = GF_NOT_SUPPORTED;
		goto exit;
	}
	sscanf(url, "ES_ID=%u", &ESID);

	if ((s32) ESID == 1 + ffd->audio_st) {
		if (ffd->audio_ch) {
			e = GF_SERVICE_ERROR;
			goto exit;
		}
		ffd->audio_ch = channel;
		e = GF_OK;
	}
	else if ((s32) ESID == 1 + ffd->video_st) {
		if (ffd->video_ch) {
			e = GF_SERVICE_ERROR;
			goto exit;
		}
		ffd->video_ch = channel;
		e = GF_OK;
	}

exit:
	gf_service_connect_ack(ffd->service, channel, e);
	return GF_OK;
}
