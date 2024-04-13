static int vrend_decode_create_object(struct vrend_decode_ctx *ctx, int length)
{
   if (length < 1)
      return EINVAL;

   uint32_t header = get_buf_entry(ctx, VIRGL_OBJ_CREATE_HEADER);
   uint32_t handle = get_buf_entry(ctx, VIRGL_OBJ_CREATE_HANDLE);
   uint8_t obj_type = (header >> 8) & 0xff;
   int ret = 0;

   if (handle == 0)
      return EINVAL;

   switch (obj_type){
   case VIRGL_OBJECT_BLEND:
      ret = vrend_decode_create_blend(ctx, handle, length);
      break;
   case VIRGL_OBJECT_DSA:
      ret = vrend_decode_create_dsa(ctx, handle, length);
      break;
   case VIRGL_OBJECT_RASTERIZER:
      ret = vrend_decode_create_rasterizer(ctx, handle, length);
      break;
   case VIRGL_OBJECT_SHADER:
      ret = vrend_decode_create_shader(ctx, handle, length);
      break;
   case VIRGL_OBJECT_VERTEX_ELEMENTS:
      ret = vrend_decode_create_ve(ctx, handle, length);
      break;
   case VIRGL_OBJECT_SURFACE:
      ret = vrend_decode_create_surface(ctx, handle, length);
      break;
   case VIRGL_OBJECT_SAMPLER_VIEW:
      ret = vrend_decode_create_sampler_view(ctx, handle, length);
      break;
   case VIRGL_OBJECT_SAMPLER_STATE:
      ret = vrend_decode_create_sampler_state(ctx, handle, length);
      break;
   case VIRGL_OBJECT_QUERY:
      ret = vrend_decode_create_query(ctx, handle, length);
      break;
   case VIRGL_OBJECT_STREAMOUT_TARGET:
      ret = vrend_decode_create_stream_output_target(ctx, handle, length);
      break;
   default:
      return EINVAL;
   }

   return ret;
}
