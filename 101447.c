void NavigationRequest::ResetForCrossDocumentRestart() {
  DCHECK(
      FrameMsg_Navigate_Type::IsSameDocument(common_params_.navigation_type));

  DCHECK(!loader_);
  navigation_handle_.reset();

  if (common_params_.navigation_type ==
      FrameMsg_Navigate_Type::HISTORY_SAME_DOCUMENT) {
    common_params_.navigation_type =
        FrameMsg_Navigate_Type::HISTORY_DIFFERENT_DOCUMENT;
  } else {
    DCHECK(common_params_.navigation_type ==
           FrameMsg_Navigate_Type::SAME_DOCUMENT);
    common_params_.navigation_type = FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT;
  }

  state_ = NOT_STARTED;
}
