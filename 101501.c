void Browser::AddNewContents(WebContents* source,
                             WebContents* new_contents,
                             WindowOpenDisposition disposition,
                             const gfx::Rect& initial_rect,
                             bool user_gesture,
                             bool* was_blocked) {
  chrome::AddWebContents(this, source, new_contents, disposition, initial_rect,
                         user_gesture, was_blocked);
}
