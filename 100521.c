RootWindowHostLinux::RootWindowHostLinux(const gfx::Rect& bounds)
    : root_window_(NULL),
      xdisplay_(base::MessagePumpX::GetDefaultXDisplay()),
      xwindow_(0),
      x_root_window_(DefaultRootWindow(xdisplay_)),
      current_cursor_(ui::kCursorNull),
      cursor_shown_(true),
      bounds_(bounds),
      focus_when_shown_(false),
      pointer_barriers_(NULL),
      image_cursors_(new ImageCursors) {
  XSetWindowAttributes swa;
  memset(&swa, 0, sizeof(swa));
  swa.background_pixmap = None;
  xwindow_ = XCreateWindow(
      xdisplay_, x_root_window_,
      bounds.x(), bounds.y(), bounds.width(), bounds.height(),
      0,               // border width
      CopyFromParent,  // depth
      InputOutput,
      CopyFromParent,  // visual
      CWBackPixmap,
      &swa);
  static_cast<DispatcherLinux*>(Env::GetInstance()->GetDispatcher())->
      WindowDispatcherCreated(xwindow_, this);

  prop_.reset(new ui::ViewProp(xwindow_, kRootWindowHostLinuxKey, this));

  long event_mask = ButtonPressMask | ButtonReleaseMask | FocusChangeMask |
                    KeyPressMask | KeyReleaseMask |
                    EnterWindowMask | LeaveWindowMask |
                    ExposureMask | VisibilityChangeMask |
                    StructureNotifyMask | PropertyChangeMask |
                    PointerMotionMask;
  XSelectInput(xdisplay_, xwindow_, event_mask);
  XFlush(xdisplay_);

  if (base::MessagePumpForUI::HasXInput2())
    ui::TouchFactory::GetInstance()->SetupXI2ForXWindow(xwindow_);

  char nodata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  XColor black;
  black.red = black.green = black.blue = 0;
  Pixmap blank = XCreateBitmapFromData(xdisplay_, xwindow_,
                                       nodata, 8, 8);
  invisible_cursor_ = XCreatePixmapCursor(xdisplay_, blank, blank,
                                          &black, &black, 0, 0);
  XFreePixmap(xdisplay_, blank);

  if (RootWindow::hide_host_cursor())
    XDefineCursor(xdisplay_, x_root_window_, invisible_cursor_);

  X11AtomCache* cache = X11AtomCache::GetInstance();
  ::Atom protocols[2];
  protocols[0] = cache->GetAtom(ui::ATOM_WM_DELETE_WINDOW);
  protocols[1] = cache->GetAtom(ui::ATOM__NET_WM_PING);
  XSetWMProtocols(xdisplay_, xwindow_, protocols, 2);

  XSetWMProperties(xdisplay_, xwindow_, NULL, NULL, NULL, 0, NULL, NULL, NULL);

  pid_t pid = getpid();
  XChangeProperty(xdisplay_,
                  xwindow_,
                  cache->GetAtom(ui::ATOM__NET_WM_PID),
                  XA_CARDINAL,
                  32,
                  PropModeReplace,
                  reinterpret_cast<unsigned char*>(&pid), 1);

  static int root_window_number = 0;
  std::string name = StringPrintf("aura_root_%d", root_window_number++);
  XStoreName(xdisplay_, xwindow_, name.c_str());
}
