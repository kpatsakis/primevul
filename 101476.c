bool V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::SetUpJpegParameters(
    int quality,
    gfx::Size coded_size) {
  DCHECK(parent_->encoder_task_runner_->BelongsToCurrentThread());

  struct v4l2_ext_controls ctrls;
  struct v4l2_ext_control ctrl;

  memset(&ctrls, 0, sizeof(ctrls));
  memset(&ctrl, 0, sizeof(ctrl));

  ctrls.ctrl_class = V4L2_CTRL_CLASS_JPEG;
  ctrls.controls = &ctrl;
  ctrls.count = 1;

  switch (output_buffer_pixelformat_) {
    case V4L2_PIX_FMT_JPEG_RAW:
      FillQuantizationTable(quality, kDefaultQuantTable[0].value,
                            quantization_table_[0].value);
      FillQuantizationTable(quality, kDefaultQuantTable[1].value,
                            quantization_table_[1].value);

      ctrl.id = V4L2_CID_JPEG_LUMA_QUANTIZATION;
      ctrl.size = kDctSize;
      ctrl.ptr = quantization_table_[0].value;
      IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_S_EXT_CTRLS, &ctrls);

      ctrl.id = V4L2_CID_JPEG_CHROMA_QUANTIZATION;
      ctrl.size = kDctSize;
      ctrl.ptr = quantization_table_[1].value;
      IOCTL_OR_ERROR_RETURN_FALSE(VIDIOC_S_EXT_CTRLS, &ctrls);

      PrepareJpegMarkers(coded_size);
      break;

    default:
      NOTREACHED();
  }

  return true;
}
