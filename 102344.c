static WORD32 ihevcd_parse_vui_parameters(bitstrm_t *ps_bitstrm,
 vui_t *ps_vui,
                                          WORD32 sps_max_sub_layers_minus1)
{
    WORD32 ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;

    BITS_PARSE("aspect_ratio_info_present_flag", ps_vui->u1_aspect_ratio_info_present_flag, ps_bitstrm, 1);

    ps_vui->u1_aspect_ratio_idc = SAR_UNUSED;
    ps_vui->u2_sar_width = 0;
    ps_vui->u2_sar_height = 0;
 if(ps_vui->u1_aspect_ratio_info_present_flag)
 {
        BITS_PARSE("aspect_ratio_idc", ps_vui->u1_aspect_ratio_idc, ps_bitstrm, 8);
 if(ps_vui->u1_aspect_ratio_idc  ==  EXTENDED_SAR)
 {
            BITS_PARSE("sar_width", ps_vui->u2_sar_width, ps_bitstrm, 16);
            BITS_PARSE("sar_height", ps_vui->u2_sar_height, ps_bitstrm, 16);
 }
 }

    BITS_PARSE("overscan_info_present_flag", ps_vui->u1_overscan_info_present_flag, ps_bitstrm, 1);
    ps_vui->u1_overscan_appropriate_flag = 0;
 if(ps_vui->u1_overscan_info_present_flag)
        BITS_PARSE("overscan_appropriate_flag", ps_vui->u1_overscan_appropriate_flag, ps_bitstrm, 1);

    BITS_PARSE("video_signal_type_present_flag", ps_vui->u1_video_signal_type_present_flag, ps_bitstrm, 1);
    ps_vui->u1_video_format = VID_FMT_UNSPECIFIED;
    ps_vui->u1_video_full_range_flag = 0;
    ps_vui->u1_colour_description_present_flag = 0;
 if(ps_vui->u1_video_signal_type_present_flag)
 {
        BITS_PARSE("video_format", ps_vui->u1_video_format, ps_bitstrm, 3);
        BITS_PARSE("video_full_range_flag", ps_vui->u1_video_full_range_flag, ps_bitstrm, 1);
        BITS_PARSE("colour_description_present_flag", ps_vui->u1_colour_description_present_flag, ps_bitstrm, 1);
        ps_vui->u1_colour_primaries = 2;
        ps_vui->u1_transfer_characteristics = 2;
 if(ps_vui->u1_colour_description_present_flag)
 {
            BITS_PARSE("colour_primaries", ps_vui->u1_colour_primaries, ps_bitstrm, 8);
            BITS_PARSE("transfer_characteristics", ps_vui->u1_transfer_characteristics, ps_bitstrm, 8);
            BITS_PARSE("matrix_coeffs", ps_vui->u1_matrix_coefficients, ps_bitstrm, 8);
 }
 }

    BITS_PARSE("chroma_loc_info_present_flag", ps_vui->u1_chroma_loc_info_present_flag, ps_bitstrm, 1);
    ps_vui->u1_chroma_sample_loc_type_top_field = 0;
    ps_vui->u1_chroma_sample_loc_type_bottom_field = 0;
 if(ps_vui->u1_chroma_loc_info_present_flag)
 {
        UEV_PARSE("chroma_sample_loc_type_top_field", ps_vui->u1_chroma_sample_loc_type_top_field, ps_bitstrm);
        UEV_PARSE("chroma_sample_loc_type_bottom_field", ps_vui->u1_chroma_sample_loc_type_bottom_field, ps_bitstrm);
 }

    BITS_PARSE("neutral_chroma_indication_flag", ps_vui->u1_neutral_chroma_indication_flag, ps_bitstrm, 1);
    BITS_PARSE("field_seq_flag", ps_vui->u1_field_seq_flag, ps_bitstrm, 1);
    BITS_PARSE("frame_field_info_present_flag", ps_vui->u1_frame_field_info_present_flag, ps_bitstrm, 1);
    BITS_PARSE("default_display_window_flag", ps_vui->u1_default_display_window_flag, ps_bitstrm, 1);
    ps_vui->u4_def_disp_win_left_offset = 0;
    ps_vui->u4_def_disp_win_right_offset = 0;
    ps_vui->u4_def_disp_win_top_offset = 0;
    ps_vui->u4_def_disp_win_bottom_offset = 0;
 if(ps_vui->u1_default_display_window_flag)
 {
        UEV_PARSE("def_disp_win_left_offset", ps_vui->u4_def_disp_win_left_offset, ps_bitstrm);
        UEV_PARSE("def_disp_win_right_offset", ps_vui->u4_def_disp_win_right_offset, ps_bitstrm);
        UEV_PARSE("def_disp_win_top_offset", ps_vui->u4_def_disp_win_top_offset, ps_bitstrm);
        UEV_PARSE("def_disp_win_bottom_offset", ps_vui->u4_def_disp_win_bottom_offset, ps_bitstrm);
 }

    BITS_PARSE("vui_timing_info_present_flag", ps_vui->u1_vui_timing_info_present_flag, ps_bitstrm, 1);
 if(ps_vui->u1_vui_timing_info_present_flag)
 {
        BITS_PARSE("vui_num_units_in_tick", ps_vui->u4_vui_num_units_in_tick, ps_bitstrm, 32);
        BITS_PARSE("vui_time_scale", ps_vui->u4_vui_time_scale, ps_bitstrm, 32);
        BITS_PARSE("vui_poc_proportional_to_timing_flag", ps_vui->u1_poc_proportional_to_timing_flag, ps_bitstrm, 1);
 if(ps_vui->u1_poc_proportional_to_timing_flag)
            UEV_PARSE("vui_num_ticks_poc_diff_one_minus1", ps_vui->u1_num_ticks_poc_diff_one_minus1, ps_bitstrm);

        BITS_PARSE("vui_hrd_parameters_present_flag", ps_vui->u1_vui_hrd_parameters_present_flag, ps_bitstrm, 1);
 if(ps_vui->u1_vui_hrd_parameters_present_flag)
            ihevcd_parse_hrd_parameters(ps_bitstrm, &ps_vui->s_vui_hrd_parameters, 1, sps_max_sub_layers_minus1);
 }

    BITS_PARSE("bitstream_restriction_flag", ps_vui->u1_bitstream_restriction_flag, ps_bitstrm, 1);
    ps_vui->u1_tiles_fixed_structure_flag = 0;
    ps_vui->u1_motion_vectors_over_pic_boundaries_flag = 1;
    ps_vui->u1_restricted_ref_pic_lists_flag = 0;
    ps_vui->u4_min_spatial_segmentation_idc = 0;
    ps_vui->u1_max_bytes_per_pic_denom = 2;
    ps_vui->u1_max_bits_per_mincu_denom = 1;
    ps_vui->u1_log2_max_mv_length_horizontal = 15;
    ps_vui->u1_log2_max_mv_length_vertical = 15;
 if(ps_vui->u1_bitstream_restriction_flag)
 {
        BITS_PARSE("tiles_fixed_structure_flag", ps_vui->u1_tiles_fixed_structure_flag, ps_bitstrm, 1);
        BITS_PARSE("motion_vectors_over_pic_boundaries_flag", ps_vui->u1_motion_vectors_over_pic_boundaries_flag, ps_bitstrm, 1);
        BITS_PARSE("restricted_ref_pic_lists_flag", ps_vui->u1_restricted_ref_pic_lists_flag, ps_bitstrm, 1);

        UEV_PARSE("min_spatial_segmentation_idc", ps_vui->u4_min_spatial_segmentation_idc, ps_bitstrm);
        UEV_PARSE("max_bytes_per_pic_denom", ps_vui->u1_max_bytes_per_pic_denom, ps_bitstrm);
        UEV_PARSE("max_bits_per_min_cu_denom", ps_vui->u1_max_bits_per_mincu_denom, ps_bitstrm);
        UEV_PARSE("log2_max_mv_length_horizontal", ps_vui->u1_log2_max_mv_length_horizontal, ps_bitstrm);
        UEV_PARSE("log2_max_mv_length_vertical", ps_vui->u1_log2_max_mv_length_vertical, ps_bitstrm);
 }

 return ret;
}
