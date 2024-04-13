int vrend_decode_block(uint32_t ctx_id, uint32_t *block, int ndw)
{
   struct vrend_decode_ctx *gdctx;
   bool bret;
   int ret;
   if (ctx_id >= VREND_MAX_CTX)
      return EINVAL;

   if (dec_ctx[ctx_id] == NULL)
      return EINVAL;

   gdctx = dec_ctx[ctx_id];

   bret = vrend_hw_switch_context(gdctx->grctx, true);
   if (bret == false)
      return EINVAL;

   gdctx->ds->buf = block;
   gdctx->ds->buf_total = ndw;
   gdctx->ds->buf_offset = 0;

   while (gdctx->ds->buf_offset < gdctx->ds->buf_total) {
      uint32_t header = gdctx->ds->buf[gdctx->ds->buf_offset];
      uint32_t len = header >> 16;

      ret = 0;
      /* check if the guest is doing something bad */
      if (gdctx->ds->buf_offset + len + 1 > gdctx->ds->buf_total) {
         vrend_report_buffer_error(gdctx->grctx, 0);
         break;
      }

      switch (header & 0xff) {
      case VIRGL_CCMD_CREATE_OBJECT:
         ret = vrend_decode_create_object(gdctx, len);
         break;
      case VIRGL_CCMD_BIND_OBJECT:
         ret = vrend_decode_bind_object(gdctx, len);
         break;
      case VIRGL_CCMD_DESTROY_OBJECT:
         ret = vrend_decode_destroy_object(gdctx, len);
         break;
      case VIRGL_CCMD_CLEAR:
         ret = vrend_decode_clear(gdctx, len);
         break;
      case VIRGL_CCMD_DRAW_VBO:
         ret = vrend_decode_draw_vbo(gdctx, len);
         break;
      case VIRGL_CCMD_SET_FRAMEBUFFER_STATE:
         ret = vrend_decode_set_framebuffer_state(gdctx, len);
         break;
      case VIRGL_CCMD_SET_VERTEX_BUFFERS:
         ret = vrend_decode_set_vertex_buffers(gdctx, len);
         break;
      case VIRGL_CCMD_RESOURCE_INLINE_WRITE:
         ret = vrend_decode_resource_inline_write(gdctx, len);
         break;
      case VIRGL_CCMD_SET_VIEWPORT_STATE:
         ret = vrend_decode_set_viewport_state(gdctx, len);
         break;
      case VIRGL_CCMD_SET_SAMPLER_VIEWS:
         ret = vrend_decode_set_sampler_views(gdctx, len);
         break;
      case VIRGL_CCMD_SET_INDEX_BUFFER:
         ret = vrend_decode_set_index_buffer(gdctx, len);
         break;
      case VIRGL_CCMD_SET_CONSTANT_BUFFER:
         ret = vrend_decode_set_constant_buffer(gdctx, len);
         break;
      case VIRGL_CCMD_SET_STENCIL_REF:
         ret = vrend_decode_set_stencil_ref(gdctx, len);
         break;
      case VIRGL_CCMD_SET_BLEND_COLOR:
         ret = vrend_decode_set_blend_color(gdctx, len);
         break;
      case VIRGL_CCMD_SET_SCISSOR_STATE:
         ret = vrend_decode_set_scissor_state(gdctx, len);
         break;
      case VIRGL_CCMD_BLIT:
         ret = vrend_decode_blit(gdctx, len);
         break;
      case VIRGL_CCMD_RESOURCE_COPY_REGION:
         ret = vrend_decode_resource_copy_region(gdctx, len);
         break;
      case VIRGL_CCMD_BIND_SAMPLER_STATES:
         ret = vrend_decode_bind_sampler_states(gdctx, len);
         break;
      case VIRGL_CCMD_BEGIN_QUERY:
         ret = vrend_decode_begin_query(gdctx, len);
         break;
      case VIRGL_CCMD_END_QUERY:
         ret = vrend_decode_end_query(gdctx, len);
         break;
      case VIRGL_CCMD_GET_QUERY_RESULT:
         ret = vrend_decode_get_query_result(gdctx, len);
         break;
      case VIRGL_CCMD_SET_POLYGON_STIPPLE:
         ret = vrend_decode_set_polygon_stipple(gdctx, len);
         break;
      case VIRGL_CCMD_SET_CLIP_STATE:
         ret = vrend_decode_set_clip_state(gdctx, len);
         break;
      case VIRGL_CCMD_SET_SAMPLE_MASK:
         ret = vrend_decode_set_sample_mask(gdctx, len);
         break;
      case VIRGL_CCMD_SET_STREAMOUT_TARGETS:
         ret = vrend_decode_set_streamout_targets(gdctx, len);
         break;
      case VIRGL_CCMD_SET_RENDER_CONDITION:
         ret = vrend_decode_set_render_condition(gdctx, len);
         break;
      case VIRGL_CCMD_SET_UNIFORM_BUFFER:
         ret = vrend_decode_set_uniform_buffer(gdctx, len);
         break;
      case VIRGL_CCMD_SET_SUB_CTX:
         ret = vrend_decode_set_sub_ctx(gdctx, len);
         break;
      case VIRGL_CCMD_CREATE_SUB_CTX:
         ret = vrend_decode_create_sub_ctx(gdctx, len);
         break;
      case VIRGL_CCMD_DESTROY_SUB_CTX:
         ret = vrend_decode_destroy_sub_ctx(gdctx, len);
         break;
      case VIRGL_CCMD_BIND_SHADER:
         ret = vrend_decode_bind_shader(gdctx, len);
         break;
      default:
         ret = EINVAL;
      }

      if (ret == EINVAL) {
         vrend_report_buffer_error(gdctx->grctx, header);
         goto out;
      }
      if (ret == ENOMEM)
         goto out;
      gdctx->ds->buf_offset += (len) + 1;
   }
   return 0;
 out:
   return ret;
}
