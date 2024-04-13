static void get_texcoords(struct vrend_resource *src_res,
                          int src_level,
                          int x1, int y1, int x2, int y2,
                          float out[4])
{
   bool normalized = src_res->base.target != PIPE_TEXTURE_RECT &&
      src_res->base.nr_samples <= 1;

   if (normalized) {
      out[0] = x1 / (float)u_minify(src_res->base.width0,  src_level);
      out[1] = y1 / (float)u_minify(src_res->base.height0, src_level);
      out[2] = x2 / (float)u_minify(src_res->base.width0,  src_level);
      out[3] = y2 / (float)u_minify(src_res->base.height0, src_level);
   } else {
      out[0] = (float) x1;
      out[1] = (float) y1;
      out[2] = (float) x2;
      out[3] = (float) y2;
   }
}
