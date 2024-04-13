static int vrend_decode_bind_object(struct vrend_decode_ctx *ctx, uint16_t length)
{
   if (length != 1)
      return EINVAL;

   uint32_t header = get_buf_entry(ctx, VIRGL_OBJ_BIND_HEADER);
   uint32_t handle = get_buf_entry(ctx, VIRGL_OBJ_BIND_HANDLE);
   uint8_t obj_type = (header >> 8) & 0xff;

   switch (obj_type) {
   case VIRGL_OBJECT_BLEND:
      vrend_object_bind_blend(ctx->grctx, handle);
      break;
   case VIRGL_OBJECT_DSA:
      vrend_object_bind_dsa(ctx->grctx, handle);
      break;
   case VIRGL_OBJECT_RASTERIZER:
      vrend_object_bind_rasterizer(ctx->grctx, handle);
      break;
   case VIRGL_OBJECT_VERTEX_ELEMENTS:
      vrend_bind_vertex_elements_state(ctx->grctx, handle);
      break;
   default:
      return EINVAL;
   }

   return 0;
}
