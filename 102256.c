static bool signal_has_si_addr(int sig) {
 switch (sig) {
 case SIGBUS:
 case SIGFPE:
 case SIGILL:
 case SIGSEGV:
 case SIGTRAP:
 return true;
 default:
 return false;
 }
}
