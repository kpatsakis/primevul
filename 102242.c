static const void* printCommand(TextOutput& out, const void* _cmd)
{
 static const size_t N = sizeof(kCommandStrings)/sizeof(kCommandStrings[0]);
 const int32_t* cmd = (const int32_t*)_cmd;
 uint32_t code = (uint32_t)*cmd++;
 size_t cmdIndex = code & 0xff;

 if (cmdIndex >= N) {
        out << "Unknown command: " << code << endl;
 return cmd;
 }
    out << kCommandStrings[cmdIndex];

 switch (code) {
 case BC_TRANSACTION:
 case BC_REPLY: {
            out << ": " << indent;
            cmd = (const int32_t *)printBinderTransactionData(out, cmd);
            out << dedent;
 } break;
 
 case BC_ACQUIRE_RESULT: {
 const int32_t res = *cmd++;
            out << ": " << res << (res ? " (SUCCESS)" : " (FAILURE)");
 } break;
 
 case BC_FREE_BUFFER: {
 const int32_t buf = *cmd++;
            out << ": buffer=" << (void*)(long)buf;
 } break;
 
 case BC_INCREFS:
 case BC_ACQUIRE:
 case BC_RELEASE:
 case BC_DECREFS: {
 const int32_t d = *cmd++;
            out << ": desc=" << d;
 } break;
 
 case BC_INCREFS_DONE:
 case BC_ACQUIRE_DONE: {
 const int32_t b = *cmd++;
 const int32_t c = *cmd++;
            out << ": target=" << (void*)(long)b << " (cookie " << (void*)(long)c << ")";
 } break;
 
 case BC_ATTEMPT_ACQUIRE: {
 const int32_t p = *cmd++;
 const int32_t d = *cmd++;
            out << ": desc=" << d << ", pri=" << p;
 } break;
 
 case BC_REQUEST_DEATH_NOTIFICATION:
 case BC_CLEAR_DEATH_NOTIFICATION: {
 const int32_t h = *cmd++;
 const int32_t c = *cmd++;
            out << ": handle=" << h << " (death cookie " << (void*)(long)c << ")";
 } break;

 case BC_DEAD_BINDER_DONE: {
 const int32_t c = *cmd++;
            out << ": death cookie " << (void*)(long)c;
 } break;

 default:
 break;
 }
 
    out << endl;
 return cmd;
}
