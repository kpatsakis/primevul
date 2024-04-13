bool V4L2JpegEncodeAccelerator::EncodedInstanceDmaBuf::Initialize() {
  device_ = V4L2Device::Create();
  gpu_memory_buffer_support_ = std::make_unique<gpu::GpuMemoryBufferSupport>();

  if (!device_) {
    VLOGF(1) << "Failed to Create V4L2Device";
    return false;
  }

  output_buffer_pixelformat_ = V4L2_PIX_FMT_JPEG_RAW;
  if (!device_->Open(V4L2Device::Type::kJpegEncoder,
                     output_buffer_pixelformat_)) {
    VLOGF(1) << "Failed to open device";
    return false;
  }

  struct v4l2_capability caps;
  const __u32 kCapsRequired = V4L2_CAP_STREAMING | V4L2_CAP_VIDEO_M2M_MPLANE;
  memset(&caps, 0, sizeof(caps));
  if (device_->Ioctl(VIDIOC_QUERYCAP, &caps) != 0) {
    VPLOGF(1) << "ioctl() failed: VIDIOC_QUERYCAP";
    return false;
  }
  if ((caps.capabilities & kCapsRequired) != kCapsRequired) {
    VLOGF(1) << "VIDIOC_QUERYCAP, caps check failed: 0x" << std::hex
             << caps.capabilities;
    return false;
  }

  return true;
}
