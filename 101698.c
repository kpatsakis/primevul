void Browser::OnDidBlockFramebust(content::WebContents* web_contents,
                                  const GURL& url) {
  TabSpecificContentSettings* content_settings =
      TabSpecificContentSettings::FromWebContents(web_contents);
  DCHECK(content_settings);
  content_settings->OnFramebustBlocked(
      url, FramebustBlockTabHelper::ClickCallback());
}
