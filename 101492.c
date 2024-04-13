void PasswordAutofillAgent::BindPendingReceiver(
    mojo::PendingAssociatedReceiver<mojom::PasswordAutofillAgent>
        pending_receiver) {
  receiver_.Bind(std::move(pending_receiver));
}
