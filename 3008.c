GF_Err stbl_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_SampleTableBox *ptr = (GF_SampleTableBox *)s;
	if (!a) return GF_OK;
	switch (a->type) {
	case GF_ISOM_BOX_TYPE_STTS:
		BOX_FIELD_ASSIGN(TimeToSample, GF_TimeToSampleBox)
		break;
	case GF_ISOM_BOX_TYPE_CTTS:
		BOX_FIELD_ASSIGN(CompositionOffset, GF_CompositionOffsetBox)
		break;
	case GF_ISOM_BOX_TYPE_CSLG:
		BOX_FIELD_ASSIGN(CompositionToDecode, GF_CompositionToDecodeBox)
		break;
	case GF_ISOM_BOX_TYPE_STSS:
		BOX_FIELD_ASSIGN(SyncSample, GF_SyncSampleBox)
		break;
	case GF_ISOM_BOX_TYPE_STSD:
		BOX_FIELD_ASSIGN(SampleDescription, GF_SampleDescriptionBox)
		break;
	case GF_ISOM_BOX_TYPE_STZ2:
	case GF_ISOM_BOX_TYPE_STSZ:
		BOX_FIELD_ASSIGN(SampleSize, GF_SampleSizeBox)
		break;
	case GF_ISOM_BOX_TYPE_STSC:
		BOX_FIELD_ASSIGN(SampleToChunk, GF_SampleToChunkBox)
		break;
	case GF_ISOM_BOX_TYPE_PADB:
		BOX_FIELD_ASSIGN(PaddingBits, GF_PaddingBitsBox)
		break;

	//WARNING: AS THIS MAY CHANGE DYNAMICALLY DURING EDIT,
	case GF_ISOM_BOX_TYPE_CO64:
	case GF_ISOM_BOX_TYPE_STCO:
		BOX_FIELD_ASSIGN(ChunkOffset, GF_Box)
		break;
	case GF_ISOM_BOX_TYPE_STSH:
		BOX_FIELD_ASSIGN(ShadowSync, GF_ShadowSyncBox)
		break;
	case GF_ISOM_BOX_TYPE_STDP:
		BOX_FIELD_ASSIGN(DegradationPriority, GF_DegradationPriorityBox)
		break;
	case GF_ISOM_BOX_TYPE_SDTP:
		BOX_FIELD_ASSIGN(SampleDep, GF_SampleDependencyTypeBox)
		break;

	case GF_ISOM_BOX_TYPE_SUBS:
		BOX_FIELD_LIST_ASSIGN(sub_samples)
		//check subsample box
		if (!is_rem) {
			GF_SubSampleInformationBox *subs = (GF_SubSampleInformationBox *)a;
			GF_SubSampleInfoEntry *ent = gf_list_get(subs->Samples, 0);
			if (!ent) {
				gf_list_rem(subs->Samples, 0);
				GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("[iso file] first entry in SubSample in track SampleTable is invalid\n"));
			}
			else if (ent->sample_delta==0) {
				GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("[iso file] first entry in SubSample in track SampleTable has sample_delta of 0, should be one. Fixing\n"));
				ent->sample_delta = 1;
			}
		}
		break;

	case GF_ISOM_BOX_TYPE_SBGP:
	case GF_ISOM_BOX_TYPE_CSGP:
		BOX_FIELD_LIST_ASSIGN(sampleGroups)
		break;
	case GF_ISOM_BOX_TYPE_SGPD:
		BOX_FIELD_LIST_ASSIGN(sampleGroupsDescription)
		break;
	case GF_ISOM_BOX_TYPE_SAIZ:
		BOX_FIELD_LIST_ASSIGN(sai_sizes)
		break;
	case GF_ISOM_BOX_TYPE_SAIO:
		BOX_FIELD_LIST_ASSIGN(sai_offsets)
		break;
	}
	return GF_OK;
}