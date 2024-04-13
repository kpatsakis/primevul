static void read_transfer_data(struct pipe_resource *res,
                               struct iovec *iov,
                               unsigned int num_iovs,
                               char *data,
                               uint32_t src_stride,
                               struct pipe_box *box,
                               uint64_t offset, bool invert)
{
   int blsize = util_format_get_blocksize(res->format);
   uint32_t size = vrend_get_iovec_size(iov, num_iovs);
   uint32_t send_size = util_format_get_nblocks(res->format, box->width,
                                              box->height) * blsize * box->depth;
   uint32_t bwx = util_format_get_nblocksx(res->format, box->width) * blsize;
   uint32_t bh = util_format_get_nblocksy(res->format, box->height);
   int h;
   uint32_t myoffset = offset;

   if ((send_size == size || bh == 1) && !invert)
      vrend_read_from_iovec(iov, num_iovs, offset, data, send_size);
   else {
      if (invert) {
         for (h = bh - 1; h >= 0; h--) {
            void *ptr = data + (h * bwx);
            vrend_read_from_iovec(iov, num_iovs, myoffset, ptr, bwx);
            myoffset += src_stride;
         }
      } else {
         for (h = 0; h < bh; h++) {
            void *ptr = data + (h * bwx);
            vrend_read_from_iovec(iov, num_iovs, myoffset, ptr, bwx);
            myoffset += src_stride;
         }
      }
   }
}
