void PageFormAnalyserLogger::Send(std::string message,
                                  ConsoleLevel level,
                                  blink::WebNode node) {
  Send(std::move(message), level, std::vector<blink::WebNode>{std::move(node)});
}
