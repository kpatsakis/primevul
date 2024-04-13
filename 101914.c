void ServiceWorkerPaymentInstrument::OnCanMakePaymentEventSkipped(
    ValidateCanMakePaymentCallback callback) {
  can_make_payment_result_ = true;
  has_enrolled_instrument_result_ = false;
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::BindOnce(std::move(callback), this, can_make_payment_result_));
}
