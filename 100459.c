GLES2DecoderImpl::GLES2DecoderImpl(ContextGroup* group)
    : GLES2Decoder(group),
      error_bits_(0),
      util_(0),  // TODO(gman): Set to actual num compress texture formats.
      pack_alignment_(4),
      unpack_alignment_(4),
      attrib_0_buffer_id_(0),
      attrib_0_size_(0),
      active_texture_unit_(0),
      black_2d_texture_id_(0),
      black_cube_texture_id_(0),
      clear_red_(0),
      clear_green_(0),
      clear_blue_(0),
      clear_alpha_(0),
      mask_red_(true),
      mask_green_(true),
      mask_blue_(true),
      mask_alpha_(true),
      clear_stencil_(0),
      mask_stencil_front_(-1),
      mask_stencil_back_(-1),
      clear_depth_(1.0f),
      mask_depth_(true),
      enable_scissor_test_(false),
      anti_aliased_(false),
      use_shader_translator_(true),
      vertex_compiler_(NULL),
      fragment_compiler_(NULL),
      validators_(group->validators()),
      depth24_stencil8_oes_supported_(false) {
  attrib_0_value_.v[0] = 0.0f;
  attrib_0_value_.v[1] = 0.0f;
  attrib_0_value_.v[2] = 0.0f;
  attrib_0_value_.v[3] = 1.0f;

  if (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2 ||
      gfx::GetGLImplementation() == gfx::kGLImplementationMockGL) {
    use_shader_translator_ = false;
  }
}
