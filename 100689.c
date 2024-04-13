static int StartDownload(content::RenderViewHost* rvh,
                         const GURL& url,
                         bool is_favicon,
                         uint32_t max_bitmap_size) {
  static int g_next_image_download_id = 0;
  rvh->Send(new ImageMsg_DownloadImage(rvh->GetRoutingID(),
                                       ++g_next_image_download_id,
                                       url,
                                       is_favicon,
                                       max_bitmap_size));
  return g_next_image_download_id;
}
