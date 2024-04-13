void PaymentRequestState::CheckHasEnrolledInstrument(StatusCallback callback) {
  DCHECK(get_all_instruments_finished_);
  bool has_enrolled_instrument_value = false;
  for (const auto& instrument : available_instruments_) {
    if (instrument->IsValidForCanMakePayment()) {
      has_enrolled_instrument_value = true;
      break;
    }
  }
  std::move(callback).Run(has_enrolled_instrument_value);
}
