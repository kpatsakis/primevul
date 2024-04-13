static void blitter_set_texcoords(struct vrend_blitter_ctx *blit_ctx,
                                  struct vrend_resource *src_res,
                                  int level,
                                  float layer, unsigned sample,
                                  int x1, int y1, int x2, int y2)
{
   float coord[4];
   float face_coord[4][2];
   int i;
   get_texcoords(src_res, level, x1, y1, x2, y2, coord);

   if (src_res->base.target == PIPE_TEXTURE_CUBE ||
       src_res->base.target == PIPE_TEXTURE_CUBE_ARRAY) {
      set_texcoords_in_vertices(coord, &face_coord[0][0], 2);
      util_map_texcoords2d_onto_cubemap((unsigned)layer % 6,
                                        /* pointer, stride in floats */
                                        &face_coord[0][0], 2,
                                        &blit_ctx->vertices[0][1][0], 8,
                                        FALSE);
   } else {
      set_texcoords_in_vertices(coord, &blit_ctx->vertices[0][1][0], 8);
   }

   switch (src_res->base.target) {
   case PIPE_TEXTURE_3D:
   {
      float r = layer / (float)u_minify(src_res->base.depth0,
                                        level);
      for (i = 0; i < 4; i++)
         blit_ctx->vertices[i][1][2] = r; /*r*/
   }
   break;

   case PIPE_TEXTURE_1D_ARRAY:
      for (i = 0; i < 4; i++)
         blit_ctx->vertices[i][1][1] = (float) layer; /*t*/
      break;

   case PIPE_TEXTURE_2D_ARRAY:
      for (i = 0; i < 4; i++) {
         blit_ctx->vertices[i][1][2] = (float) layer;  /*r*/
         blit_ctx->vertices[i][1][3] = (float) sample; /*q*/
      }
      break;
   case PIPE_TEXTURE_CUBE_ARRAY:
      for (i = 0; i < 4; i++)
         blit_ctx->vertices[i][1][3] = (float) ((unsigned)layer / 6); /*w*/
      break;
   case PIPE_TEXTURE_2D:
      for (i = 0; i < 4; i++) {
         blit_ctx->vertices[i][1][3] = (float) sample; /*r*/
      }
      break;
   default:;
   }
}
