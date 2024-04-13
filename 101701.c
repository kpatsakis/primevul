void Browser::RequestKeyboardLock(WebContents* web_contents,
                                  bool esc_key_locked) {
  exclusive_access_manager_->keyboard_lock_controller()->RequestKeyboardLock(
      web_contents, esc_key_locked);
}
