PDFiumEngine::PDFiumEngine(PDFEngine::Client* client)
    : client_(client),
      current_zoom_(1.0),
      current_rotation_(0),
      doc_(nullptr),
      form_(nullptr),
      defer_page_unload_(false),
      selecting_(false),
      mouse_down_state_(PDFiumPage::NONSELECTABLE_AREA,
                        PDFiumPage::LinkTarget()),
      in_form_text_area_(false),
      editable_form_text_area_(false),
      mouse_left_button_down_(false),
      permissions_(0),
      permissions_handler_revision_(-1),
      fpdf_availability_(nullptr),
      last_page_mouse_down_(-1),
      most_visible_page_(-1),
      called_do_document_action_(false),
      render_grayscale_(false),
      render_annots_(true) {
  find_factory_.Initialize(this);
  password_factory_.Initialize(this);

  file_access_.m_FileLen = 0;
  file_access_.m_GetBlock = &GetBlock;
  file_access_.m_Param = this;

  file_availability_.version = 1;
  file_availability_.IsDataAvail = &IsDataAvail;
  file_availability_.engine = this;

  download_hints_.version = 1;
  download_hints_.AddSegment = &AddSegment;
  download_hints_.engine = this;

  FPDF_FORMFILLINFO::version = 1;
  FPDF_FORMFILLINFO::m_pJsPlatform = this;
  FPDF_FORMFILLINFO::Release = nullptr;
  FPDF_FORMFILLINFO::FFI_Invalidate = Form_Invalidate;
  FPDF_FORMFILLINFO::FFI_OutputSelectedRect = Form_OutputSelectedRect;
  FPDF_FORMFILLINFO::FFI_SetCursor = Form_SetCursor;
  FPDF_FORMFILLINFO::FFI_SetTimer = Form_SetTimer;
  FPDF_FORMFILLINFO::FFI_KillTimer = Form_KillTimer;
  FPDF_FORMFILLINFO::FFI_GetLocalTime = Form_GetLocalTime;
  FPDF_FORMFILLINFO::FFI_OnChange = Form_OnChange;
  FPDF_FORMFILLINFO::FFI_GetPage = Form_GetPage;
  FPDF_FORMFILLINFO::FFI_GetCurrentPage = Form_GetCurrentPage;
  FPDF_FORMFILLINFO::FFI_GetRotation = Form_GetRotation;
  FPDF_FORMFILLINFO::FFI_ExecuteNamedAction = Form_ExecuteNamedAction;
  FPDF_FORMFILLINFO::FFI_SetTextFieldFocus = Form_SetTextFieldFocus;
  FPDF_FORMFILLINFO::FFI_DoURIAction = Form_DoURIAction;
  FPDF_FORMFILLINFO::FFI_DoGoToAction = Form_DoGoToAction;
#if defined(PDF_ENABLE_XFA)
  FPDF_FORMFILLINFO::version = 2;
  FPDF_FORMFILLINFO::FFI_EmailTo = Form_EmailTo;
  FPDF_FORMFILLINFO::FFI_DisplayCaret = Form_DisplayCaret;
  FPDF_FORMFILLINFO::FFI_SetCurrentPage = Form_SetCurrentPage;
  FPDF_FORMFILLINFO::FFI_GetCurrentPageIndex = Form_GetCurrentPageIndex;
  FPDF_FORMFILLINFO::FFI_GetPageViewRect = Form_GetPageViewRect;
  FPDF_FORMFILLINFO::FFI_GetPlatform = Form_GetPlatform;
  FPDF_FORMFILLINFO::FFI_PageEvent = nullptr;
  FPDF_FORMFILLINFO::FFI_PopupMenu = Form_PopupMenu;
  FPDF_FORMFILLINFO::FFI_PostRequestURL = Form_PostRequestURL;
  FPDF_FORMFILLINFO::FFI_PutRequestURL = Form_PutRequestURL;
  FPDF_FORMFILLINFO::FFI_UploadTo = Form_UploadTo;
  FPDF_FORMFILLINFO::FFI_DownloadFromURL = Form_DownloadFromURL;
  FPDF_FORMFILLINFO::FFI_OpenFile = Form_OpenFile;
  FPDF_FORMFILLINFO::FFI_GotoURL = Form_GotoURL;
  FPDF_FORMFILLINFO::FFI_GetLanguage = Form_GetLanguage;
#endif  // defined(PDF_ENABLE_XFA)
  IPDF_JSPLATFORM::version = 3;
  IPDF_JSPLATFORM::app_alert = Form_Alert;
  IPDF_JSPLATFORM::app_beep = Form_Beep;
  IPDF_JSPLATFORM::app_response = Form_Response;
  IPDF_JSPLATFORM::Doc_getFilePath = Form_GetFilePath;
  IPDF_JSPLATFORM::Doc_mail = Form_Mail;
  IPDF_JSPLATFORM::Doc_print = Form_Print;
  IPDF_JSPLATFORM::Doc_submitForm = Form_SubmitForm;
  IPDF_JSPLATFORM::Doc_gotoPage = Form_GotoPage;
  IPDF_JSPLATFORM::Field_browse = nullptr;

  IFSDK_PAUSE::version = 1;
  IFSDK_PAUSE::user = nullptr;
  IFSDK_PAUSE::NeedToPauseNow = Pause_NeedToPauseNow;

#if defined(OS_LINUX)
  pp::Instance* instance = client_->GetPluginInstance();
  if (instance)
    g_last_instance_id = instance->pp_instance();
#endif
}
