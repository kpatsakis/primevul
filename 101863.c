ScopedMessagePipeHandle Core::CreatePartialMessagePipe(ports::PortRef* peer) {
  RequestContext request_context;
  ports::PortRef local_port;
  GetNodeController()->node()->CreatePortPair(&local_port, peer);
  MojoHandle handle = AddDispatcher(new MessagePipeDispatcher(
      GetNodeController(), local_port, kUnknownPipeIdForDebug, 0));
  return ScopedMessagePipeHandle(MessagePipeHandle(handle));
}
