ScopedHandle WrapPlatformFile(base::PlatformFile platform_file) {
  MojoPlatformHandle platform_handle;
  platform_handle.struct_size = sizeof(MojoPlatformHandle);
  platform_handle.type = kPlatformFileHandleType;
  platform_handle.value = PlatformHandleValueFromPlatformFile(platform_file);

  MojoHandle mojo_handle;
  MojoResult result = MojoWrapPlatformHandle(&platform_handle, &mojo_handle);
  CHECK_EQ(result, MOJO_RESULT_OK);

  return ScopedHandle(Handle(mojo_handle));
}
