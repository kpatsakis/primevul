MojoResult Core::CommitSerializedMessageContents(
    MojoMessageHandle message_handle,
    uint32_t final_payload_size,
    void** buffer,
    uint32_t* buffer_size) {
  if (!message_handle)
    return MOJO_RESULT_INVALID_ARGUMENT;
  RequestContext request_context;
  auto* message = reinterpret_cast<ports::UserMessageEvent*>(message_handle)
                      ->GetMessage<UserMessageImpl>();
  MojoResult rv = message->CommitSerializedContents(final_payload_size);
  if (rv != MOJO_RESULT_OK)
    return rv;

  if (buffer)
    *buffer = message->user_payload();
  if (buffer_size) {
    *buffer_size =
        base::checked_cast<uint32_t>(message->user_payload_capacity());
  }
  return MOJO_RESULT_OK;
}
