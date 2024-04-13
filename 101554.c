bool OmniboxViewViews::TextAndUIDirectionMatch() const {
  return (GetRenderText()->GetDisplayTextDirection() ==
          base::i18n::RIGHT_TO_LEFT) == base::i18n::IsRTL();
}
