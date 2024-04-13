static bool check_iov_bounds(struct vrend_resource *res,
                             const struct vrend_transfer_info *info,
                             struct iovec *iov, int num_iovs)
{
   GLuint send_size;
   GLuint iovsize = vrend_get_iovec_size(iov, num_iovs);
   GLuint valid_stride, valid_layer_stride;

   /* validate the send size */
   valid_stride = util_format_get_stride(res->base.format, info->box->width);
   if (info->stride) {
      /* only validate passed in stride for boxes with height */
      if (info->box->height > 1) {
         if (info->stride < valid_stride)
            return false;
         valid_stride = info->stride;
      }
   }

   valid_layer_stride = util_format_get_2d_size(res->base.format, valid_stride,
                                                info->box->height);

   /* layer stride only makes sense for 3d,cube and arrays */
   if (info->layer_stride) {
      if ((res->base.target != PIPE_TEXTURE_3D &&
           res->base.target != PIPE_TEXTURE_CUBE &&
           res->base.target != PIPE_TEXTURE_1D_ARRAY &&
           res->base.target != PIPE_TEXTURE_2D_ARRAY &&
           res->base.target != PIPE_TEXTURE_CUBE_ARRAY))
         return false;

      /* only validate passed in layer_stride for boxes with depth */
      if (info->box->depth > 1) {
         if (info->layer_stride < valid_layer_stride)
            return false;
         valid_layer_stride = info->layer_stride;
      }
   }

   send_size = valid_layer_stride * info->box->depth;
   if (iovsize < info->offset)
      return false;
   if (iovsize < send_size)
      return false;
   if (iovsize < info->offset + send_size)
      return false;

   return true;
}
