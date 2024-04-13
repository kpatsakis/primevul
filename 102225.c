static void* start_handler(void* data)
{
 struct fuse_handler* handler = data;
    handle_fuse_requests(handler);
 return NULL;
}
