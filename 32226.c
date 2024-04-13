int vrend_renderer_init(struct vrend_if_cbs *cbs, uint32_t flags)
{
   int gl_ver;
   virgl_gl_context gl_context;
   struct virgl_gl_ctx_param ctx_params;

   if (!vrend_state.inited) {
      vrend_state.inited = true;
      vrend_object_init_resource_table();
      vrend_clicbs = cbs;
   }

   ctx_params.shared = false;
   ctx_params.major_ver = VREND_GL_VER_MAJOR;
   ctx_params.minor_ver = VREND_GL_VER_MINOR;

   gl_context = vrend_clicbs->create_gl_context(0, &ctx_params);
   vrend_clicbs->make_current(0, gl_context);
   gl_ver = epoxy_gl_version();

   vrend_state.gl_major_ver = gl_ver / 10;
   vrend_state.gl_minor_ver = gl_ver % 10;
   if (gl_ver > 30 && !epoxy_has_gl_extension("GL_ARB_compatibility")) {
      fprintf(stderr, "gl_version %d - core profile enabled\n", gl_ver);
      vrend_state.use_core_profile = 1;
   } else {
      fprintf(stderr, "gl_version %d - compat profile\n", gl_ver);
   }

   if (epoxy_has_gl_extension("GL_ARB_robustness"))
      vrend_state.have_robustness = true;
   else
      fprintf(stderr,"WARNING: running without ARB robustness in place may crash\n");

   if (epoxy_has_gl_extension("GL_MESA_pack_invert"))
      vrend_state.have_mesa_invert = true;
   if (gl_ver >= 43 || epoxy_has_gl_extension("GL_ARB_vertex_attrib_binding"))
      vrend_state.have_vertex_attrib_binding = true;
   if (gl_ver >= 33 || epoxy_has_gl_extension("GL_ARB_sampler_objects"))
      vrend_state.have_samplers = true;
   if (gl_ver >= 33 || epoxy_has_gl_extension("GL_ARB_shader_bit_encoding"))
      vrend_state.have_bit_encoding = true;
   if (gl_ver >= 31)
      vrend_state.have_gl_prim_restart = true;
   else if (epoxy_has_gl_extension("GL_NV_primitive_restart"))
      vrend_state.have_nv_prim_restart = true;
   if (gl_ver >= 40 || epoxy_has_gl_extension("GL_ARB_transform_feedback2"))
      vrend_state.have_tf2 = true;

   if (epoxy_has_gl_extension("GL_EXT_framebuffer_multisample") && epoxy_has_gl_extension("GL_ARB_texture_multisample")) {
      vrend_state.have_multisample = true;
      if (epoxy_has_gl_extension("GL_EXT_framebuffer_multisample_blit_scaled"))
         vrend_state.have_ms_scaled_blit = true;
   }

   /* callbacks for when we are cleaning up the object table */
   vrend_resource_set_destroy_callback(vrend_destroy_resource_object);
   vrend_object_set_destroy_callback(VIRGL_OBJECT_QUERY, vrend_destroy_query_object);
   vrend_object_set_destroy_callback(VIRGL_OBJECT_SURFACE, vrend_destroy_surface_object);
   vrend_object_set_destroy_callback(VIRGL_OBJECT_SHADER, vrend_destroy_shader_object);
   vrend_object_set_destroy_callback(VIRGL_OBJECT_SAMPLER_VIEW, vrend_destroy_sampler_view_object);
   vrend_object_set_destroy_callback(VIRGL_OBJECT_STREAMOUT_TARGET, vrend_destroy_so_target_object);
   vrend_object_set_destroy_callback(VIRGL_OBJECT_SAMPLER_STATE, vrend_destroy_sampler_state_object);
   vrend_object_set_destroy_callback(VIRGL_OBJECT_VERTEX_ELEMENTS, vrend_destroy_vertex_elements_object);

   vrend_build_format_list();

   vrend_clicbs->destroy_gl_context(gl_context);
   list_inithead(&vrend_state.fence_list);
   list_inithead(&vrend_state.fence_wait_list);
   list_inithead(&vrend_state.waiting_query_list);
   list_inithead(&vrend_state.active_ctx_list);
   /* create 0 context */
   vrend_renderer_context_create_internal(0, 0, NULL);

   vrend_state.eventfd = -1;
   if (flags & VREND_USE_THREAD_SYNC) {
      vrend_renderer_use_threaded_sync();
   }

   return 0;
}
