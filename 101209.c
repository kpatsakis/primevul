void ExecuteScript(blink::WebFrame* frame,
                   const char* script_format,
                   const base::Value& parameters) {
  std::string json;
  base::JSONWriter::Write(parameters, &json);
  std::string script = base::StringPrintf(script_format, json.c_str());
  frame->executeScript(blink::WebString(base::UTF8ToUTF16(script)));
}
