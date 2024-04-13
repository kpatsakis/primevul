  ~ImageCursors() {
    std::map<int, Cursor>::const_iterator it;
    for (it = cursors_.begin(); it != cursors_.end(); ++it)
      ui::UnrefCustomXCursor(it->second);
  }
