WORD32 ihevcd_set_num_cores(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{
 ihevcd_cxa_ctl_set_num_cores_ip_t *ps_ip;
 ihevcd_cxa_ctl_set_num_cores_op_t *ps_op;
 codec_t *ps_codec = (codec_t *)ps_codec_obj->pv_codec_handle;

    ps_ip = (ihevcd_cxa_ctl_set_num_cores_ip_t *)pv_api_ip;
    ps_op = (ihevcd_cxa_ctl_set_num_cores_op_t *)pv_api_op;

#ifdef MULTICORE
    ps_codec->i4_num_cores = ps_ip->u4_num_cores;
#else
    ps_codec->i4_num_cores = 1;
#endif
    ps_op->u4_error_code = 0;
 return IV_SUCCESS;
}
