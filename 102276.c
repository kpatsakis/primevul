static int getCallingPid() {
 return IPCThreadState::self()->getCallingPid();
}
