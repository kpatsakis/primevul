static void dump_stack_segment(
 Backtrace* backtrace, log_t* log, uintptr_t* sp, size_t words, int label) {
 for (size_t i = 0; i < words; i++) {
 word_t stack_content;
 if (!backtrace->ReadWord(*sp, &stack_content)) {
 break;
 }

 const backtrace_map_t* map = backtrace->FindMap(stack_content);
 const char* map_name;
 if (!map) {
      map_name = "";
 } else {
      map_name = map->name.c_str();
 }
 uintptr_t offset = 0;
    std::string func_name(backtrace->GetFunctionName(stack_content, &offset));
 if (!func_name.empty()) {
 if (!i && label >= 0) {
 if (offset) {
          _LOG(log, logtype::STACK, "    #%02d  %" PRIPTR "  %" PRIPTR "  %s (%s+%" PRIuPTR ")\n",
               label, *sp, stack_content, map_name, func_name.c_str(), offset);
 } else {
          _LOG(log, logtype::STACK, "    #%02d  %" PRIPTR "  %" PRIPTR "  %s (%s)\n",
               label, *sp, stack_content, map_name, func_name.c_str());
 }
 } else {
 if (offset) {
          _LOG(log, logtype::STACK, "         %" PRIPTR "  %" PRIPTR "  %s (%s+%" PRIuPTR ")\n",
 *sp, stack_content, map_name, func_name.c_str(), offset);
 } else {
          _LOG(log, logtype::STACK, "         %" PRIPTR "  %" PRIPTR "  %s (%s)\n",
 *sp, stack_content, map_name, func_name.c_str());
 }
 }
 } else {
 if (!i && label >= 0) {
        _LOG(log, logtype::STACK, "    #%02d  %" PRIPTR "  %" PRIPTR "  %s\n",
             label, *sp, stack_content, map_name);
 } else {
        _LOG(log, logtype::STACK, "         %" PRIPTR "  %" PRIPTR "  %s\n",
 *sp, stack_content, map_name);
 }
 }

 *sp += sizeof(word_t);
 }
}
