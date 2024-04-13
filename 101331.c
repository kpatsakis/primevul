  const BookmarkNode* GetNode(const std::string& name) {
    return model_->GetMostRecentlyAddedUserNodeForURL(GURL(base_path() + name));
  }
