void vrend_renderer_get_rect(int res_handle, struct iovec *iov, unsigned int num_iovs,
                             uint32_t offset, int x, int y, int width, int height)
{
   struct vrend_resource *res = vrend_resource_lookup(res_handle, 0);
   struct vrend_transfer_info transfer_info;
   struct pipe_box box;
   int elsize;

   memset(&transfer_info, 0, sizeof(transfer_info));

   elsize = util_format_get_blocksize(res->base.format);
   box.x = x;
   box.y = y;
   box.z = 0;
   box.width = width;
   box.height = height;
   box.depth = 1;

   transfer_info.box = &box;

   transfer_info.stride = util_format_get_nblocksx(res->base.format, res->base.width0) * elsize;
   transfer_info.offset = offset;
   transfer_info.handle = res->handle;
   transfer_info.iovec = iov;
   transfer_info.iovec_cnt = num_iovs;
   vrend_renderer_transfer_iov(&transfer_info, VREND_TRANSFER_READ);
}
