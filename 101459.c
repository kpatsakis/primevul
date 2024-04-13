GrBackendFormat SkiaOutputSurfaceImpl::GetGrBackendFormatForTexture(
    ResourceFormat resource_format,
    uint32_t gl_texture_target,
    const base::Optional<gpu::VulkanYCbCrInfo>& ycbcr_info) {
  if (!is_using_vulkan_) {
    DCHECK(!ycbcr_info);
    unsigned int texture_storage_format = gpu::GetGrGLBackendTextureFormat(
        impl_on_gpu_->GetFeatureInfo(), resource_format);

    return GrBackendFormat::MakeGL(texture_storage_format, gl_texture_target);
  } else {
#if BUILDFLAG(ENABLE_VULKAN)
    if (!ycbcr_info) {
      DCHECK(resource_format != YVU_420 && resource_format != YUV_420_BIPLANAR);
      return GrBackendFormat::MakeVk(ToVkFormat(resource_format));
    }

    GrVkYcbcrConversionInfo gr_ycbcr_info =
        CreateGrVkYcbcrConversionInfo(dependency_->GetVulkanContextProvider()
                                          ->GetDeviceQueue()
                                          ->GetVulkanPhysicalDevice(),
                                      VK_IMAGE_TILING_OPTIMAL, ycbcr_info);
    return GrBackendFormat::MakeVk(gr_ycbcr_info);
#else
    NOTREACHED();
    return GrBackendFormat();
#endif
  }
}
