  static void HandleCrash(int signo, siginfo_t* siginfo, void* context) {
    SandboxedHandler* state = Get();
    state->HandleCrashNonFatal(signo, siginfo, context);
    Signals::RestoreHandlerAndReraiseSignalOnReturn(
        siginfo, state->old_actions_.ActionForSignal(signo));
  }
