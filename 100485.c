PPB_ImageData_Impl::PPB_ImageData_Impl(PP_Instance instance,
                                       ImageDataType type)
    : Resource(::ppapi::OBJECT_IS_IMPL, instance),
      format_(PP_IMAGEDATAFORMAT_BGRA_PREMUL),
      width_(0),
      height_(0) {
  switch (type) {
    case PLATFORM:
      backend_.reset(new ImageDataPlatformBackend);
      return;
    case NACL:
      backend_.reset(new ImageDataNaClBackend);
      return;
  }
  NOTREACHED();
}
