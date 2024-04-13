void MojoVideoEncodeAcceleratorService::Initialize(
    VideoPixelFormat input_format,
    const gfx::Size& input_visible_size,
    VideoCodecProfile output_profile,
    uint32_t initial_bitrate,
    mojom::VideoEncodeAcceleratorClientPtr client,
    InitializeCallback success_callback) {
  DVLOG(1) << __func__
           << " input_format=" << VideoPixelFormatToString(input_format)
           << ", input_visible_size=" << input_visible_size.ToString()
           << ", output_profile=" << GetProfileName(output_profile)
           << ", initial_bitrate=" << initial_bitrate;
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  DCHECK(!encoder_);
  DCHECK_EQ(PIXEL_FORMAT_I420, input_format) << "Only I420 format supported";

  if (!client) {
    DLOG(ERROR) << __func__ << "null |client|";
    std::move(success_callback).Run(false);
    return;
  }
  vea_client_ = std::move(client);

  if (input_visible_size.width() > limits::kMaxDimension ||
      input_visible_size.height() > limits::kMaxDimension ||
      input_visible_size.GetArea() > limits::kMaxCanvas) {
    DLOG(ERROR) << __func__ << "too large input_visible_size "
                << input_visible_size.ToString();
    std::move(success_callback).Run(false);
    return;
  }

  encoder_ =
      create_vea_callback_.Run(input_format, input_visible_size, output_profile,
                               initial_bitrate, this, gpu_preferences_);
  if (!encoder_) {
    DLOG(ERROR) << __func__ << " Error creating or initializing VEA";
    std::move(success_callback).Run(false);
    return;
  }


  std::move(success_callback).Run(true);
  return;
}
