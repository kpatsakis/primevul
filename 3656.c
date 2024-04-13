GF_Err hinf_on_child_box(GF_Box *s, GF_Box *a, Bool is_rem)
{
	GF_HintInfoBox *hinf = (GF_HintInfoBox *)s;
	switch (a->type) {
	case GF_ISOM_BOX_TYPE_MAXR:
		if (!is_rem) {
			u32 i=0;
			GF_MAXRBox *maxR;
			while ((maxR = (GF_MAXRBox *)gf_list_enum(hinf->child_boxes, &i))) {
				if ((maxR->type==GF_ISOM_BOX_TYPE_MAXR) && (maxR->granularity == ((GF_MAXRBox *)a)->granularity))
					ERROR_ON_DUPLICATED_BOX(a, s)
			}
		}
		break;
	}
	return GF_OK;
}