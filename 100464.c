void GLES2DecoderTest::CheckReadPixelsOutOfRange(
    GLint in_read_x, GLint in_read_y,
    GLsizei in_read_width, GLsizei in_read_height,
    bool init) {
  const GLsizei kWidth = 5;
  const GLsizei kHeight = 3;
  const GLint kBytesPerPixel = 3;
  const GLint kPackAlignment = 4;
  const GLenum kFormat = GL_RGB;
  static const int8 kSrcPixels[kWidth * kHeight * kBytesPerPixel] = {
    12, 13, 14, 18, 19, 18, 19, 12, 13, 14, 18, 19, 18, 19, 13,
    29, 28, 23, 22, 21, 22, 21, 29, 28, 23, 22, 21, 22, 21, 28,
    31, 34, 39, 37, 32, 37, 32, 31, 34, 39, 37, 32, 37, 32, 34,
  };

  ClearSharedMemory();

  if (init) {
    DoBindTexture(GL_TEXTURE_2D, client_texture_id_, kServiceTextureId);
    DoTexImage2D(GL_TEXTURE_2D, 0, kFormat, kWidth, kHeight, 0,
                 kFormat, GL_UNSIGNED_BYTE, 0, 0);
    DoBindFramebuffer(GL_FRAMEBUFFER, client_framebuffer_id_,
                      kServiceFramebufferId);
  }

  EXPECT_CALL(*gl_, GetFramebufferAttachmentParameterivEXT(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
      _))
      .WillOnce(SetArgumentPointee<3>(GL_TEXTURE))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetFramebufferAttachmentParameterivEXT(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
            _))
      .WillOnce(SetArgumentPointee<3>(kServiceTextureId))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetFramebufferAttachmentParameterivEXT(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,
      _))
      .WillOnce(SetArgumentPointee<3>(0))
      .RetiresOnSaturation();
  EXPECT_CALL(*gl_, GetFramebufferAttachmentParameterivEXT(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE,
      _))
      .WillOnce(SetArgumentPointee<3>(0))
      .RetiresOnSaturation();

  ReadPixelsEmulator emu(
      kWidth, kHeight, kBytesPerPixel, kSrcPixels, kPackAlignment);
  typedef ReadPixels::Result Result;
  Result* result = GetSharedMemoryAs<Result*>();
  uint32 result_shm_id = kSharedMemoryId;
  uint32 result_shm_offset = kSharedMemoryOffset;
  uint32 pixels_shm_id = kSharedMemoryId;
  uint32 pixels_shm_offset = kSharedMemoryOffset + sizeof(*result);
  void* dest = &result[1];
  EXPECT_CALL(*gl_, GetError())
     .WillOnce(Return(GL_NO_ERROR))
     .WillOnce(Return(GL_NO_ERROR))
     .RetiresOnSaturation();
  GLint read_x = std::max(0, in_read_x);
  GLint read_y = std::max(0, in_read_y);
  GLint read_end_x = std::max(0, std::min(kWidth, in_read_x + in_read_width));
  GLint read_end_y = std::max(0, std::min(kHeight, in_read_y + in_read_height));
  GLint read_width = read_end_x - read_x;
  GLint read_height = read_end_y - read_y;
  if (read_width > 0 && read_height > 0) {
    for (GLint yy = read_y; yy < read_end_y; ++yy) {
      EXPECT_CALL(
          *gl_, ReadPixels(read_x, yy, read_width, 1,
                           kFormat, GL_UNSIGNED_BYTE, _))
          .WillOnce(Invoke(&emu, &ReadPixelsEmulator::ReadPixels))
          .RetiresOnSaturation();
    }
  }
  ReadPixels cmd;
  cmd.Init(in_read_x, in_read_y, in_read_width, in_read_height,
           kFormat, GL_UNSIGNED_BYTE,
           pixels_shm_id, pixels_shm_offset,
           result_shm_id, result_shm_offset);
  EXPECT_EQ(error::kNoError, ExecuteCmd(cmd));

  GLint unpadded_row_size = emu.ComputeImageDataSize(in_read_width, 1);
  scoped_array<int8> zero(new int8[unpadded_row_size]);
  scoped_array<int8> pack(new int8[kPackAlignment]);
  memset(zero.get(), 0, unpadded_row_size);
  memset(pack.get(), kInitialMemoryValue, kPackAlignment);
  for (GLint yy = 0; yy < in_read_height; ++yy) {
    const int8* row = static_cast<const int8*>(
        emu.ComputePackAlignmentAddress(0, yy, in_read_width, dest));
    GLint y = in_read_y + yy;
    if (y < 0 || y >= kHeight) {
      EXPECT_EQ(0, memcmp(zero.get(), row, unpadded_row_size));
    } else {
      GLint num_left_pixels = std::max(-in_read_x, 0);
      GLint num_left_bytes = num_left_pixels * kBytesPerPixel;
      EXPECT_EQ(0, memcmp(zero.get(), row, num_left_bytes));

      GLint num_right_pixels = std::max(in_read_x + in_read_width - kWidth, 0);
      GLint num_right_bytes = num_right_pixels * kBytesPerPixel;
      EXPECT_EQ(0, memcmp(zero.get(),
                            row + unpadded_row_size - num_right_bytes,
                            num_right_bytes));

      GLint x = std::max(in_read_x, 0);
      GLint num_middle_pixels =
          std::max(in_read_width - num_left_pixels - num_right_pixels, 0);
      EXPECT_TRUE(emu.CompareRowSegment(
          x, y, num_middle_pixels, row + num_left_bytes));
    }

    if (yy != in_read_height - 1) {
      GLint num_padding_bytes =
          (kPackAlignment - 1) - (unpadded_row_size % kPackAlignment);
      EXPECT_EQ(0,
                memcmp(pack.get(), row + unpadded_row_size, num_padding_bytes));
    }
  }
}
