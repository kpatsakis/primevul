GF_Err EncodeBIFSChunk(GF_SceneManager *ctx, char *bifsOutputFile, GF_Err (*AUCallback)(GF_ISOSample *))
{
	GF_Err			e;
	char *data;
	u32 data_len;
	GF_BifsEncoder *bifsenc;
	GF_InitialObjectDescriptor *iod;
	u32 i, j, count;
	GF_StreamContext *sc;
	GF_ESD *esd;
	Bool encode_names, delete_bcfg;
	GF_BIFSConfig *bcfg;
	GF_AUContext		*au;
	char szRad[GF_MAX_PATH], *ext;
	char szName[1024];
	FILE *f;

	strcpy(szRad, bifsOutputFile);
	ext = strrchr(szRad, '.');
	if (ext) ext[0] = 0;


	/* step3: encoding all AUs in ctx->streams starting at AU index 1 (0 is SceneReplace from previous context) */
	bifsenc = gf_bifs_encoder_new(ctx->scene_graph);
	e = GF_OK;

	iod = (GF_InitialObjectDescriptor *) ctx->root_od;
	/*if no iod check we only have one bifs*/
	if (!iod) {
		count = 0;
		for (i=0; i<gf_list_count(ctx->streams); i++) {
			sc = gf_list_get(ctx->streams, i);
			if (sc->streamType == GF_STREAM_OD) count++;
		}
		if (!iod && count>1) return GF_NOT_SUPPORTED;
	}

	count = gf_list_count(ctx->streams);
	for (i=0; i<count; i++) {
		u32 nbb;
		GF_StreamContext *sc = gf_list_get(ctx->streams, i);
		esd = NULL;
		if (sc->streamType != GF_STREAM_SCENE) continue;

		esd = NULL;
		if (iod) {
			for (j=0; j<gf_list_count(iod->ESDescriptors); j++) {
				esd = gf_list_get(iod->ESDescriptors, j);
				if (esd->decoderConfig && esd->decoderConfig->streamType == GF_STREAM_SCENE) {
					if (!sc->ESID) sc->ESID = esd->ESID;
					if (sc->ESID == esd->ESID) {
						break;
					}
				}
				/*special BIFS direct import from NHNT*/
				else if (gf_list_count(iod->ESDescriptors)==1) {
					sc->ESID = esd->ESID;
					break;
				}
				esd = NULL;
			}
		}

		if (!esd) {
			esd = gf_odf_desc_esd_new(2);
			if (!esd) return GF_OUT_OF_MEM;
			gf_odf_desc_del((GF_Descriptor *) esd->decoderConfig->decoderSpecificInfo);
			esd->decoderConfig->decoderSpecificInfo = NULL;
			esd->ESID = sc->ESID;
			esd->decoderConfig->streamType = GF_STREAM_SCENE;
		}
		if (!esd->decoderConfig) return GF_OUT_OF_MEM;

		/*should NOT happen (means inputctx is not properly setup)*/
		if (!esd->decoderConfig->decoderSpecificInfo) {
			bcfg = (GF_BIFSConfig*)gf_odf_desc_new(GF_ODF_BIFS_CFG_TAG);
			delete_bcfg = 1;
		}
		/*regular retrieve from ctx*/
		else if (esd->decoderConfig->decoderSpecificInfo->tag == GF_ODF_BIFS_CFG_TAG) {
			bcfg = (GF_BIFSConfig *)esd->decoderConfig->decoderSpecificInfo;
			delete_bcfg = 0;
		}
		/*should not happen either (unless loading from MP4 in which case BIFSc is not decoded)*/
		else {
			bcfg = gf_odf_get_bifs_config(esd->decoderConfig->decoderSpecificInfo, esd->decoderConfig->objectTypeIndication);
			delete_bcfg = 1;
		}
		/*NO CHANGE TO BIFSC otherwise the generated update will not match the input context*/
		nbb = GetNbBits(ctx->max_node_id);
		if (!bcfg->nodeIDbits) bcfg->nodeIDbits=nbb;
		if (bcfg->nodeIDbits<nbb) fprintf(stderr, "Warning: BIFSConfig.NodeIDBits TOO SMALL\n");

		nbb = GetNbBits(ctx->max_route_id);
		if (!bcfg->routeIDbits) bcfg->routeIDbits = nbb;
		if (bcfg->routeIDbits<nbb) fprintf(stderr, "Warning: BIFSConfig.RouteIDBits TOO SMALL\n");

		nbb = GetNbBits(ctx->max_proto_id);
		if (!bcfg->protoIDbits) bcfg->protoIDbits=nbb;
		if (bcfg->protoIDbits<nbb) fprintf(stderr, "Warning: BIFSConfig.ProtoIDBits TOO SMALL\n");

		/*this is the real pb, not stored in cfg or file level, set at EACH replaceScene*/
		encode_names = 0;

		/* The BIFS Config that is passed here should be the BIFSConfig from the IOD */
		gf_bifs_encoder_new_stream(bifsenc, sc->ESID, bcfg, encode_names, 0);
		if (delete_bcfg) gf_odf_desc_del((GF_Descriptor *)bcfg);

		/*setup MP4 track*/
		if (!esd->slConfig) esd->slConfig = (GF_SLConfig *) gf_odf_desc_new(GF_ODF_SLC_TAG);
		if (sc->timeScale) esd->slConfig->timestampResolution = sc->timeScale;
		if (!esd->slConfig->timestampResolution) esd->slConfig->timestampResolution = 1000;
		esd->ESID = sc->ESID;
		gf_bifs_encoder_get_config(bifsenc, sc->ESID, &data, &data_len);

		if (esd->decoderConfig->decoderSpecificInfo) gf_odf_desc_del((GF_Descriptor *) esd->decoderConfig->decoderSpecificInfo);
		esd->decoderConfig->decoderSpecificInfo = (GF_DefaultDescriptor *) gf_odf_desc_new(GF_ODF_DSI_TAG);
		esd->decoderConfig->decoderSpecificInfo->data = data;
		esd->decoderConfig->decoderSpecificInfo->dataLength = data_len;
		esd->decoderConfig->objectTypeIndication = gf_bifs_encoder_get_version(bifsenc, sc->ESID);

		for (j=1; j<gf_list_count(sc->AUs); j++) {
			char *data;
			u32 data_len;
			au = gf_list_get(sc->AUs, j);
			e = gf_bifs_encode_au(bifsenc, sc->ESID, au->commands, &data, &data_len);
			if (data) {
				sprintf(szName, "%s%02d.bifs", szRad, j);
				f = gf_fopen(szName, "wb");
				gf_fwrite(data, data_len, 1, f);
				gf_fclose(f);
				gf_free(data);
			}
		}
	}
	gf_bifs_encoder_del(bifsenc);
	return e;
}
