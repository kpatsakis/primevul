static int check_resource_valid(struct vrend_renderer_resource_create_args *args)
{
   /* do not accept handle 0 */
   if (args->handle == 0)
      return -1;

   /* limit the target */
   if (args->target >= PIPE_MAX_TEXTURE_TYPES)
      return -1;

   if (args->format >= VIRGL_FORMAT_MAX)
      return -1;

   /* only texture 2d and 2d array can have multiple samples */
   if (args->nr_samples > 1) {
      if (args->target != PIPE_TEXTURE_2D && args->target != PIPE_TEXTURE_2D_ARRAY)
         return -1;
      /* multisample can't have miplevels */
      if (args->last_level > 0)
         return -1;
   }

   if (args->last_level > 0) {
      /* buffer and rect textures can't have mipmaps */
      if (args->target == PIPE_BUFFER || args->target == PIPE_TEXTURE_RECT)
         return -1;
      if (args->last_level > (floor(log2(MAX2(args->width, args->height))) + 1))
         return -1;
   }
   if (args->flags != 0 && args->flags != VIRGL_RESOURCE_Y_0_TOP)
      return -1;

   if (args->flags & VIRGL_RESOURCE_Y_0_TOP)
      if (args->target != PIPE_TEXTURE_2D && args->target != PIPE_TEXTURE_RECT)
         return -1;

   /* array size for array textures only */
   if (args->target == PIPE_TEXTURE_CUBE) {
      if (args->array_size != 6)
         return -1;
   } else if (args->target == PIPE_TEXTURE_CUBE_ARRAY) {
      if (args->array_size % 6)
         return -1;
   } else if (args->array_size > 1) {
      if (args->target != PIPE_TEXTURE_2D_ARRAY &&
          args->target != PIPE_TEXTURE_1D_ARRAY)
         return -1;
   }

   if (args->bind == 0 ||
       args->bind == VREND_RES_BIND_CUSTOM ||
       args->bind == VREND_RES_BIND_INDEX_BUFFER ||
       args->bind == VREND_RES_BIND_STREAM_OUTPUT ||
       args->bind == VREND_RES_BIND_VERTEX_BUFFER ||
       args->bind == VREND_RES_BIND_CONSTANT_BUFFER) {
      if (args->target != PIPE_BUFFER)
         return -1;
      if (args->height != 1 || args->depth != 1)
         return -1;
   } else {
      if (!((args->bind & VREND_RES_BIND_SAMPLER_VIEW) ||
            (args->bind & VREND_RES_BIND_DEPTH_STENCIL) ||
            (args->bind & VREND_RES_BIND_RENDER_TARGET) ||
            (args->bind & VREND_RES_BIND_CURSOR)))
         return -1;

      if (args->target == PIPE_TEXTURE_2D ||
          args->target == PIPE_TEXTURE_RECT ||
          args->target == PIPE_TEXTURE_CUBE ||
          args->target == PIPE_TEXTURE_2D_ARRAY ||
          args->target == PIPE_TEXTURE_CUBE_ARRAY) {
         if (args->depth != 1)
            return -1;
      }
      if (args->target == PIPE_TEXTURE_1D ||
          args->target == PIPE_TEXTURE_1D_ARRAY) {
         if (args->height != 1 || args->depth != 1)
            return -1;
      }
   }
   return 0;
}
