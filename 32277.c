static int vrend_decode_create_rasterizer(struct vrend_decode_ctx *ctx, uint32_t handle, uint16_t length)
{
   struct pipe_rasterizer_state *rs_state;
   uint32_t tmp;

   if (length != VIRGL_OBJ_RS_SIZE)
      return EINVAL;

   rs_state = CALLOC_STRUCT(pipe_rasterizer_state);
   if (!rs_state)
      return ENOMEM;

   tmp = get_buf_entry(ctx, VIRGL_OBJ_RS_S0);
#define ebit(name, bit) rs_state->name = (tmp >> bit) & 0x1
#define emask(name, bit, mask) rs_state->name = (tmp >> bit) & mask

   ebit(flatshade, 0);
   ebit(depth_clip, 1);
   ebit(clip_halfz, 2);
   ebit(rasterizer_discard, 3);
   ebit(flatshade_first, 4);
   ebit(light_twoside, 5);
   ebit(sprite_coord_mode, 6);
   ebit(point_quad_rasterization, 7);
   emask(cull_face, 8, 0x3);
   emask(fill_front, 10, 0x3);
   emask(fill_back, 12, 0x3);
   ebit(scissor, 14);
   ebit(front_ccw, 15);
   ebit(clamp_vertex_color, 16);
   ebit(clamp_fragment_color, 17);
   ebit(offset_line, 18);
   ebit(offset_point, 19);
   ebit(offset_tri, 20);
   ebit(poly_smooth, 21);
   ebit(poly_stipple_enable, 22);
   ebit(point_smooth, 23);
   ebit(point_size_per_vertex, 24);
   ebit(multisample, 25);
   ebit(line_smooth, 26);
   ebit(line_stipple_enable, 27);
   ebit(line_last_pixel, 28);
   ebit(half_pixel_center, 29);
   ebit(bottom_edge_rule, 30);
   rs_state->point_size = uif(get_buf_entry(ctx, VIRGL_OBJ_RS_POINT_SIZE));
   rs_state->sprite_coord_enable = get_buf_entry(ctx, VIRGL_OBJ_RS_SPRITE_COORD_ENABLE);
   tmp = get_buf_entry(ctx, VIRGL_OBJ_RS_S3);
   emask(line_stipple_pattern, 0, 0xffff);
   emask(line_stipple_factor, 16, 0xff);
   emask(clip_plane_enable, 24, 0xff);

   rs_state->line_width = uif(get_buf_entry(ctx, VIRGL_OBJ_RS_LINE_WIDTH));
   rs_state->offset_units = uif(get_buf_entry(ctx, VIRGL_OBJ_RS_OFFSET_UNITS));
   rs_state->offset_scale = uif(get_buf_entry(ctx, VIRGL_OBJ_RS_OFFSET_SCALE));
   rs_state->offset_clamp = uif(get_buf_entry(ctx, VIRGL_OBJ_RS_OFFSET_CLAMP));

   tmp = vrend_renderer_object_insert(ctx->grctx, rs_state, sizeof(struct pipe_rasterizer_state), handle,
                                      VIRGL_OBJECT_RASTERIZER);
   if (tmp == 0) {
      FREE(rs_state);
      return ENOMEM;
   }
   return 0;
}
