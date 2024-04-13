status_t SoftHEVC::setNumCores() {
 ivdext_ctl_set_num_cores_ip_t s_set_cores_ip;
 ivdext_ctl_set_num_cores_op_t s_set_cores_op;
    IV_API_CALL_STATUS_T status;
    s_set_cores_ip.e_cmd = IVD_CMD_VIDEO_CTL;
    s_set_cores_ip.e_sub_cmd = IVDEXT_CMD_CTL_SET_NUM_CORES;
    s_set_cores_ip.u4_num_cores = MIN(mNumCores, CODEC_MAX_NUM_CORES);
    s_set_cores_ip.u4_size = sizeof(ivdext_ctl_set_num_cores_ip_t);
    s_set_cores_op.u4_size = sizeof(ivdext_ctl_set_num_cores_op_t);
    ALOGV("Set number of cores to %u", s_set_cores_ip.u4_num_cores);
    status = ivdec_api_function(mCodecCtx, (void *)&s_set_cores_ip,
 (void *)&s_set_cores_op);
 if (IV_SUCCESS != status) {
        ALOGE("Error in setting number of cores: 0x%x",
                s_set_cores_op.u4_error_code);
 return UNKNOWN_ERROR;
 }
 return OK;
}
