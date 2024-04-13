bool Camera3Device::isOpaqueInputSizeSupported(uint32_t width, uint32_t height) {
 for (uint32_t i = 0; i < mSupportedOpaqueInputSizes.size(); i++) {
 Size size = mSupportedOpaqueInputSizes[i];
 if (size.width == width && size.height == height) {
 return true;
 }
 }

 return false;
}
