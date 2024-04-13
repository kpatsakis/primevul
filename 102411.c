WORD32 ihevcd_set_degrade(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{
 ihevcd_cxa_ctl_degrade_ip_t *ps_ip;
 ihevcd_cxa_ctl_degrade_op_t *ps_op;
 codec_t *ps_codec = (codec_t *)ps_codec_obj->pv_codec_handle;

    ps_ip = (ihevcd_cxa_ctl_degrade_ip_t *)pv_api_ip;
    ps_op = (ihevcd_cxa_ctl_degrade_op_t *)pv_api_op;

    ps_codec->i4_degrade_type = ps_ip->i4_degrade_type;
    ps_codec->i4_nondegrade_interval = ps_ip->i4_nondegrade_interval;
    ps_codec->i4_degrade_pics = ps_ip->i4_degrade_pics;

    ps_op->u4_error_code = 0;
    ps_codec->i4_degrade_pic_cnt = 0;

 return IV_SUCCESS;
}
