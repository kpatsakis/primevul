GF_Err stbl_box_read(GF_Box *s, GF_BitStream *bs)
{
	GF_Err e;
	//we need to parse DegPrior in a special way
	GF_SampleTableBox *ptr = (GF_SampleTableBox *)s;

	e = gf_isom_box_array_read(s, bs);
	if (e) return e;

	if (!ptr->SyncSample)
		ptr->no_sync_found = 1;

	ptr->nb_sgpd_in_stbl = gf_list_count(ptr->sampleGroupsDescription);
	ptr->nb_stbl_boxes = gf_list_count(ptr->child_boxes);

	if (gf_bs_get_cookie(bs) & GF_ISOM_BS_COOKIE_CLONE_TRACK)
		return GF_OK;
//	return GF_OK;

#define CHECK_BOX(_name) \
	if (!ptr->_name) {\
		GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[iso file] Mandatory box %s is missing\n", #_name)); \
		return GF_ISOM_INVALID_FILE; \
	}

	CHECK_BOX(SampleToChunk)
	CHECK_BOX(SampleSize)
	CHECK_BOX(ChunkOffset)
	CHECK_BOX(TimeToSample)

	//sanity check
	if (ptr->SampleSize->sampleCount) {
		if (!ptr->TimeToSample->nb_entries || !ptr->SampleToChunk->nb_entries)
			return GF_ISOM_INVALID_FILE;
	}
	u32 i, max_chunks=0;
	if (ptr->ChunkOffset->type == GF_ISOM_BOX_TYPE_STCO) {
		max_chunks = ((GF_ChunkOffsetBox *)ptr->ChunkOffset)->nb_entries;
	}
	else if (ptr->ChunkOffset->type == GF_ISOM_BOX_TYPE_CO64) {
		max_chunks = ((GF_ChunkOffsetBox *)ptr->ChunkOffset)->nb_entries;
	}

	//sanity check on stsc vs chunk offset tables
	for (i=0; i<ptr->SampleToChunk->nb_entries; i++) {
		GF_StscEntry *ent = &ptr->SampleToChunk->entries[i];
		if (!i && (ent->firstChunk!=1)) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[iso file] first_chunk of first entry shall be 1 but is %u\n", ent->firstChunk));
			return GF_ISOM_INVALID_FILE;
		}
		if (ptr->SampleToChunk->entries[i].firstChunk > max_chunks) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[iso file] first_chunk is %u but number of chunks defined %u\n", ptr->SampleToChunk->entries[i].firstChunk, max_chunks));
			return GF_ISOM_INVALID_FILE;
		}
		if (i+1 == ptr->SampleToChunk->nb_entries) break;
		GF_StscEntry *next_ent = &ptr->SampleToChunk->entries[i+1];
		if (next_ent->firstChunk < ent->firstChunk) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[iso file] first_chunk (%u) for entry %u is greater than first_chunk (%u) for entry %u\n", i+1, ent->firstChunk, i+2, next_ent->firstChunk));
			return GF_ISOM_INVALID_FILE;
		}
	}
	return GF_OK;
}