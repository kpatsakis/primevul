const std::string& GetExtensionIdForMutedTab(content::WebContents* contents) {
  DCHECK_EQ(GetTabAudioMutedReason(contents) != TabMutedReason::EXTENSION,
            LastMuteMetadata::FromWebContents(contents)->extension_id.empty());
  return LastMuteMetadata::FromWebContents(contents)->extension_id;
}
