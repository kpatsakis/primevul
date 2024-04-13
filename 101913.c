 ServiceWorkerPaymentInstrument::CreatePaymentRequestEventData() {
   mojom::PaymentRequestEventDataPtr event_data =
      mojom::PaymentRequestEventData::New();

  event_data->top_origin = top_origin_;
  event_data->payment_request_origin = frame_origin_;

  if (spec_->details().id.has_value())
    event_data->payment_request_id = spec_->details().id.value();

  event_data->total = spec_->details().total->amount.Clone();

  std::unordered_set<std::string> supported_methods;
  if (needs_installation_) {
    supported_methods.insert(installable_enabled_method_);
  } else {
    supported_methods.insert(stored_payment_app_info_->enabled_methods.begin(),
                             stored_payment_app_info_->enabled_methods.end());
  }
  DCHECK(spec_->details().modifiers);
  for (const auto& modifier : *spec_->details().modifiers) {
    if (base::ContainsKey(supported_methods,
                          modifier->method_data->supported_method)) {
      event_data->modifiers.emplace_back(modifier.Clone());
    }
  }

  for (const auto& data : spec_->method_data()) {
    if (base::ContainsKey(supported_methods, data->supported_method)) {
      event_data->method_data.push_back(data.Clone());
    }
  }

  return event_data;
}
