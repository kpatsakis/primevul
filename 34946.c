jbig2_get_region_segment_info(Jbig2RegionSegmentInfo *info, const uint8_t *segment_data)
{
    /* 7.4.1 */
    info->width = jbig2_get_int32(segment_data);
    info->height = jbig2_get_int32(segment_data + 4);
    info->x = jbig2_get_int32(segment_data + 8);
    info->y = jbig2_get_int32(segment_data + 12);
    info->flags = segment_data[16];
    info->op = (Jbig2ComposeOp)(info->flags & 0x7);
}
