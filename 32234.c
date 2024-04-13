void vrend_renderer_resource_detach_iov(int res_handle,
                                        struct iovec **iov_p,
                                        int *num_iovs_p)
{
   struct vrend_resource *res;
   res = vrend_resource_lookup(res_handle, 0);
   if (!res) {
      return;
   }
   if (iov_p)
      *iov_p = res->iov;
   if (num_iovs_p)
      *num_iovs_p = res->num_iovs;

   res->iov = NULL;
   res->num_iovs = 0;
}
