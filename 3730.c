GF_Err traf_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_TrackFragmentBox *ptr = (GF_TrackFragmentBox *)s;

	switch (a->type) {
	case GF_ISOM_BOX_TYPE_TFHD:
		BOX_FIELD_ASSIGN(tfhd, GF_TrackFragmentHeaderBox)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_TRUN:
		BOX_FIELD_LIST_ASSIGN(TrackRuns)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_SDTP:
		BOX_FIELD_ASSIGN(sdtp, GF_SampleDependencyTypeBox)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_TFDT:
		BOX_FIELD_ASSIGN(tfdt, GF_TFBaseMediaDecodeTimeBox)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_SUBS:
		BOX_FIELD_LIST_ASSIGN(sub_samples)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_SBGP:
	case GF_ISOM_BOX_TYPE_CSGP:
		BOX_FIELD_LIST_ASSIGN(sampleGroups)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_SGPD:
		BOX_FIELD_LIST_ASSIGN(sampleGroupsDescription)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_SAIZ:
		BOX_FIELD_LIST_ASSIGN(sai_sizes)
		return GF_OK;
	case GF_ISOM_BOX_TYPE_SAIO:
		BOX_FIELD_LIST_ASSIGN(sai_offsets)
		return GF_OK;
	//we will throw an error if both PIFF_PSEC and SENC are found. Not such files seen yet
	case GF_ISOM_BOX_TYPE_UUID:
		if ( ((GF_UUIDBox *)a)->internal_4cc==GF_ISOM_BOX_UUID_PSEC) {
			BOX_FIELD_ASSIGN(sample_encryption, GF_SampleEncryptionBox)
			if (!is_rem)
				ptr->sample_encryption->traf = ptr;
			return GF_OK;
		} else if ( ((GF_UUIDBox *)a)->internal_4cc==GF_ISOM_BOX_UUID_TFXD) {
			BOX_FIELD_ASSIGN(tfxd, GF_MSSTimeExtBox)
			return GF_OK;
		} else if ( ((GF_UUIDBox *)a)->internal_4cc==GF_ISOM_BOX_UUID_TFRF) {
			BOX_FIELD_ASSIGN(tfrf, GF_MSSTimeRefBox)
			return GF_OK;
		} else {
			return GF_OK;
		}
	case GF_ISOM_BOX_TYPE_SENC:
		BOX_FIELD_ASSIGN(sample_encryption, GF_SampleEncryptionBox)
		if (!is_rem)
			ptr->sample_encryption->traf = ptr;
		return GF_OK;
	}
	return GF_OK;
}