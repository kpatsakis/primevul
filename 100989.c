TestTransactionConsumer::TestTransactionConsumer(
    RequestPriority priority,
    HttpTransactionFactory* factory)
    : state_(IDLE), error_(OK) {
  factory->CreateTransaction(priority, &trans_);
  ++quit_counter_;
}
