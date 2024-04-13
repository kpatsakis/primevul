int vrend_renderer_resource_create(struct vrend_renderer_resource_create_args *args, struct iovec *iov, uint32_t num_iovs)
{
   struct vrend_resource *gr;
   int level;
   int ret;

   ret = check_resource_valid(args);
   if (ret)
      return EINVAL;

   gr = (struct vrend_resource *)CALLOC_STRUCT(vrend_texture);
   if (!gr)
      return ENOMEM;

   gr->handle = args->handle;
   gr->iov = iov;
   gr->num_iovs = num_iovs;
   gr->base.width0 = args->width;
   gr->base.height0 = args->height;
   gr->base.depth0 = args->depth;
   gr->base.format = args->format;
   gr->base.target = args->target;
   gr->base.last_level = args->last_level;
   gr->base.nr_samples = args->nr_samples;
   gr->base.array_size = args->array_size;

   if (args->flags & VIRGL_RESOURCE_Y_0_TOP)
      gr->y_0_top = true;

   pipe_reference_init(&gr->base.reference, 1);

   if (args->bind == VREND_RES_BIND_CUSTOM) {
      /* custom should only be for buffers */
      gr->ptr = malloc(args->width);
      if (!gr->ptr) {
         FREE(gr);
         return ENOMEM;
      }
   } else if (args->bind == VREND_RES_BIND_INDEX_BUFFER) {
      gr->target = GL_ELEMENT_ARRAY_BUFFER_ARB;
      glGenBuffersARB(1, &gr->id);
      glBindBufferARB(gr->target, gr->id);
      glBufferData(gr->target, args->width, NULL, GL_STREAM_DRAW);
   } else if (args->bind == VREND_RES_BIND_STREAM_OUTPUT) {
      gr->target = GL_TRANSFORM_FEEDBACK_BUFFER;
      glGenBuffersARB(1, &gr->id);
      glBindBuffer(gr->target, gr->id);
      glBufferData(gr->target, args->width, NULL, GL_STREAM_DRAW);
   } else if (args->bind == VREND_RES_BIND_VERTEX_BUFFER) {
      gr->target = GL_ARRAY_BUFFER_ARB;
      glGenBuffersARB(1, &gr->id);
      glBindBufferARB(gr->target, gr->id);
      glBufferData(gr->target, args->width, NULL, GL_STREAM_DRAW);
   } else if (args->bind == VREND_RES_BIND_CONSTANT_BUFFER) {
      gr->target = GL_UNIFORM_BUFFER;
      glGenBuffersARB(1, &gr->id);
      glBindBufferARB(gr->target, gr->id);
      glBufferData(gr->target, args->width, NULL, GL_STREAM_DRAW);
   } else if (args->target == PIPE_BUFFER && args->bind == 0) {
      gr->target = GL_ARRAY_BUFFER_ARB;
      glGenBuffersARB(1, &gr->id);
      glBindBufferARB(gr->target, gr->id);
      glBufferData(gr->target, args->width, NULL, GL_STREAM_DRAW);
   } else if (args->target == PIPE_BUFFER && (args->bind & VREND_RES_BIND_SAMPLER_VIEW)) {
      GLenum internalformat;
      /* need to check GL version here */
      if (epoxy_has_gl_extension("GL_ARB_texture_buffer_object")) {
         gr->target = GL_TEXTURE_BUFFER;
         glGenBuffersARB(1, &gr->id);
         glBindBufferARB(gr->target, gr->id);
         glGenTextures(1, &gr->tbo_tex_id);
         glBufferData(gr->target, args->width, NULL, GL_STREAM_DRAW);

         glBindTexture(gr->target, gr->tbo_tex_id);
         internalformat = tex_conv_table[args->format].internalformat;
         glTexBuffer(gr->target, internalformat, gr->id);
      } else {
         gr->target = GL_PIXEL_PACK_BUFFER_ARB;
         glGenBuffersARB(1, &gr->id);
         glBindBufferARB(gr->target, gr->id);
         glBufferData(gr->target, args->width, NULL, GL_STREAM_DRAW);
      }
   } else {
      struct vrend_texture *gt = (struct vrend_texture *)gr;
      GLenum internalformat, glformat, gltype;
      gr->target = tgsitargettogltarget(args->target, args->nr_samples);
      glGenTextures(1, &gr->id);
      glBindTexture(gr->target, gr->id);

      internalformat = tex_conv_table[args->format].internalformat;
      glformat = tex_conv_table[args->format].glformat;
      gltype = tex_conv_table[args->format].gltype;
      if (internalformat == 0) {
         fprintf(stderr,"unknown format is %d\n", args->format);
         FREE(gr);
         return EINVAL;
      }

      if (args->nr_samples > 1) {
         if (gr->target == GL_TEXTURE_2D_MULTISAMPLE) {
            glTexImage2DMultisample(gr->target, args->nr_samples,
                                    internalformat, args->width, args->height,
                                    GL_TRUE);
         } else {
            glTexImage3DMultisample(gr->target, args->nr_samples,
                                    internalformat, args->width, args->height, args->array_size,
                                    GL_TRUE);
         }

      } else if (gr->target == GL_TEXTURE_CUBE_MAP) {
         int i;
         for (i = 0; i < 6; i++) {
            GLenum ctarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
            for (level = 0; level <= args->last_level; level++) {
               unsigned mwidth = u_minify(args->width, level);
               unsigned mheight = u_minify(args->height, level);
               glTexImage2D(ctarget, level, internalformat, mwidth, mheight, 0, glformat,
                            gltype, NULL);
            }
         }
      } else if (gr->target == GL_TEXTURE_3D ||
                 gr->target == GL_TEXTURE_2D_ARRAY ||
                 gr->target == GL_TEXTURE_CUBE_MAP_ARRAY) {
         for (level = 0; level <= args->last_level; level++) {
            unsigned depth_param = (gr->target == GL_TEXTURE_2D_ARRAY || gr->target == GL_TEXTURE_CUBE_MAP_ARRAY) ? args->array_size : u_minify(args->depth, level);
            unsigned mwidth = u_minify(args->width, level);
            unsigned mheight = u_minify(args->height, level);
            glTexImage3D(gr->target, level, internalformat, mwidth, mheight, depth_param, 0,
                         glformat,
                         gltype, NULL);
         }
      } else if (gr->target == GL_TEXTURE_1D) {
         for (level = 0; level <= args->last_level; level++) {
            unsigned mwidth = u_minify(args->width, level);
            glTexImage1D(gr->target, level, internalformat, mwidth, 0,
                         glformat,
                         gltype, NULL);
         }
      } else {
         for (level = 0; level <= args->last_level; level++) {
            unsigned mwidth = u_minify(args->width, level);
            unsigned mheight = u_minify(args->height, level);
            glTexImage2D(gr->target, level, internalformat, mwidth, gr->target == GL_TEXTURE_1D_ARRAY ? args->array_size : mheight, 0, glformat,
                         gltype, NULL);
         }
      }

      gt->state.max_lod = -1;
      gt->cur_swizzle_r = gt->cur_swizzle_g = gt->cur_swizzle_b = gt->cur_swizzle_a = -1;
   }

   ret = vrend_resource_insert(gr, args->handle);
   if (ret == 0) {
      vrend_renderer_resource_destroy(gr, true);
      return ENOMEM;
   }
   return 0;
}
