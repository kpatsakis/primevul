static WORD32 ihevcd_parse_hrd_parameters(bitstrm_t *ps_bitstrm,
 hrd_params_t *ps_hrd,
                                          WORD32 common_info_present_flag,
                                          WORD32 max_num_sub_layers_minus1)
{
    WORD32 ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 i;

    ps_hrd->u1_nal_hrd_parameters_present_flag = 0;
    ps_hrd->u1_vcl_hrd_parameters_present_flag = 0;

    ps_hrd->u1_sub_pic_cpb_params_present_flag = 0;

    ps_hrd->u1_tick_divisor_minus2 = 0;
    ps_hrd->u1_du_cpb_removal_delay_increment_length_minus1 = 0;
    ps_hrd->u1_sub_pic_cpb_params_in_pic_timing_sei_flag = 0;
    ps_hrd->u1_dpb_output_delay_du_length_minus1 = 0;

    ps_hrd->u4_bit_rate_scale = 0;
    ps_hrd->u4_cpb_size_scale = 0;
    ps_hrd->u4_cpb_size_du_scale = 0;

    ps_hrd->u1_initial_cpb_removal_delay_length_minus1 = 23;
    ps_hrd->u1_au_cpb_removal_delay_length_minus1 = 23;
    ps_hrd->u1_dpb_output_delay_length_minus1 = 23;

 if(common_info_present_flag)
 {
        BITS_PARSE("nal_hrd_parameters_present_flag", ps_hrd->u1_nal_hrd_parameters_present_flag, ps_bitstrm, 1);
        BITS_PARSE("vcl_hrd_parameters_present_flag", ps_hrd->u1_vcl_hrd_parameters_present_flag, ps_bitstrm, 1);

 if(ps_hrd->u1_nal_hrd_parameters_present_flag  ||  ps_hrd->u1_vcl_hrd_parameters_present_flag)
 {
            BITS_PARSE("sub_pic_cpb_params_present_flag", ps_hrd->u1_sub_pic_cpb_params_present_flag, ps_bitstrm, 1);
 if(ps_hrd->u1_sub_pic_cpb_params_present_flag)
 {
                BITS_PARSE("tick_divisor_minus2", ps_hrd->u1_tick_divisor_minus2, ps_bitstrm, 8);
                BITS_PARSE("du_cpb_removal_delay_increment_length_minus1", ps_hrd->u1_du_cpb_removal_delay_increment_length_minus1, ps_bitstrm, 5);
                BITS_PARSE("sub_pic_cpb_params_in_pic_timing_sei_flag", ps_hrd->u1_sub_pic_cpb_params_in_pic_timing_sei_flag, ps_bitstrm, 1);
                BITS_PARSE("dpb_output_delay_du_length_minus1", ps_hrd->u1_dpb_output_delay_du_length_minus1, ps_bitstrm, 5);
 }

            BITS_PARSE("bit_rate_scale", ps_hrd->u4_bit_rate_scale, ps_bitstrm, 4);
            BITS_PARSE("cpb_size_scale", ps_hrd->u4_cpb_size_scale, ps_bitstrm, 4);
 if(ps_hrd->u1_sub_pic_cpb_params_present_flag)
                BITS_PARSE("cpb_size_du_scale", ps_hrd->u4_cpb_size_du_scale, ps_bitstrm, 4);

            BITS_PARSE("initial_cpb_removal_delay_length_minus1", ps_hrd->u1_initial_cpb_removal_delay_length_minus1, ps_bitstrm, 5);
            BITS_PARSE("au_cpb_removal_delay_length_minus1", ps_hrd->u1_au_cpb_removal_delay_length_minus1, ps_bitstrm, 5);
            BITS_PARSE("dpb_output_delay_length_minus1", ps_hrd->u1_dpb_output_delay_length_minus1, ps_bitstrm, 5);
 }
 }


 for(i = 0; i <= max_num_sub_layers_minus1; i++)
 {
        BITS_PARSE("fixed_pic_rate_general_flag[ i ]", ps_hrd->au1_fixed_pic_rate_general_flag[i], ps_bitstrm, 1);

        ps_hrd->au1_fixed_pic_rate_within_cvs_flag[i] = 1;
        ps_hrd->au1_elemental_duration_in_tc_minus1[i] = 0;
        ps_hrd->au1_low_delay_hrd_flag[i] = 0;
        ps_hrd->au1_cpb_cnt_minus1[i] = 0;

 if(!ps_hrd->au1_fixed_pic_rate_general_flag[i])
            BITS_PARSE("fixed_pic_rate_within_cvs_flag[ i ]", ps_hrd->au1_fixed_pic_rate_within_cvs_flag[i], ps_bitstrm, 1);

 if(ps_hrd->au1_fixed_pic_rate_within_cvs_flag[i])
 {
            UEV_PARSE("elemental_duration_in_tc_minus1[ i ]", ps_hrd->au1_elemental_duration_in_tc_minus1[i], ps_bitstrm);
 }
 else
 {
            BITS_PARSE("low_delay_hrd_flag[ i ]", ps_hrd->au1_low_delay_hrd_flag[i], ps_bitstrm, 1);
 }

 if(!ps_hrd->au1_low_delay_hrd_flag[i])
            UEV_PARSE("cpb_cnt_minus1[ i ]", ps_hrd->au1_cpb_cnt_minus1[i], ps_bitstrm);

 if(ps_hrd->u1_nal_hrd_parameters_present_flag)
            ihevcd_parse_sub_layer_hrd_parameters(ps_bitstrm,
 &ps_hrd->as_sub_layer_hrd_params[i],
                                                  ps_hrd->au1_cpb_cnt_minus1[i],
                                                  ps_hrd->u1_sub_pic_cpb_params_present_flag);

 if(ps_hrd->u1_vcl_hrd_parameters_present_flag)
            ihevcd_parse_sub_layer_hrd_parameters(ps_bitstrm,
 &ps_hrd->as_sub_layer_hrd_params[i],
                                                  ps_hrd->au1_cpb_cnt_minus1[i],
                                                  ps_hrd->u1_sub_pic_cpb_params_present_flag);
 }

 return ret;
}
