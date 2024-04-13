void gf_sm_update_bitwrapper_buffer(GF_Node *node, const char *fileName)
{
	u32 data_size = 0;
	char *data = NULL;
	char *buffer;
	M_BitWrapper *bw = (M_BitWrapper *)node;

	if (!bw->buffer.buffer) return;
	buffer = bw->buffer.buffer;
	if (!strnicmp(buffer, "file://", 7)) {
		char *url = gf_url_concatenate(fileName, buffer+7);
		if (url) {
			FILE *f = gf_fopen(url, "rb");
			if (f) {
				fseek(f, 0, SEEK_END);
				data_size = (u32) ftell(f);
				fseek(f, 0, SEEK_SET);
				data = gf_malloc(sizeof(char)*data_size);
				if (data) {
					if (fread(data, 1, data_size, f) != data_size) {
						GF_LOG(GF_LOG_ERROR, GF_LOG_SCENE, ("[Scene Manager] error reading bitwrapper file %s\n", url));
					}
				}
				gf_fclose(f);
			}
			gf_free(url);
		}
	} else {
		Bool base_64 = 0;
		if (!strnicmp(buffer, "data:application/octet-string", 29)) {
			char *sep = strchr(bw->buffer.buffer, ',');
			base_64 = strstr(bw->buffer.buffer, ";base64") ? 1 : 0;
			if (sep) buffer = sep+1;
		}

		if (base_64) {
			data_size = 2 * (u32) strlen(buffer);
			data = (char*)gf_malloc(sizeof(char)*data_size);
			if (data)
				data_size = gf_base64_decode(buffer, (u32) strlen(buffer), data, data_size);
		} else {
			u32 i, c;
			char s[3];
			data_size = (u32) strlen(buffer) / 3;
			data = (char*)gf_malloc(sizeof(char) * data_size);
			if (data) {
				s[2] = 0;
				for (i=0; i<data_size; i++) {
					s[0] = buffer[3*i+1];
					s[1] = buffer[3*i+2];
					sscanf(s, "%02X", &c);
					data[i] = (unsigned char) c;
				}
			}
		}
	}
	gf_free(bw->buffer.buffer);
	bw->buffer.buffer = NULL;
	bw->buffer_len = 0;
	if (data) {
		bw->buffer.buffer = data;
		bw->buffer_len = data_size;
	}

}
