void PDFiumEngine::Form_KillTimer(FPDF_FORMFILLINFO* param, int timer_id) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  engine->timers_.erase(timer_id);
}
