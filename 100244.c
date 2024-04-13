void AutoFillManager::FillBillingFormField(const CreditCard* credit_card,
                                           AutoFillType type,
                                           webkit_glue::FormField* field) {
  DCHECK(credit_card);
  DCHECK(type.group() == AutoFillType::ADDRESS_BILLING);
  DCHECK(field);

  string16 billing_address = credit_card->billing_address();
  if (!billing_address.empty()) {
    AutoFillProfile* profile = NULL;
    const std::vector<AutoFillProfile*>& profiles = personal_data_->profiles();
    for (std::vector<AutoFillProfile*>::const_iterator iter = profiles.begin();
         iter != profiles.end(); ++iter) {
      if ((*iter)->Label() == billing_address) {
        profile = *iter;
        break;
      }
    }

    if (profile) {
      FillFormField(profile, type, field);
    }
  }
}
