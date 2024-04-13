js_run_command (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject,
                size_t argumentCount, const JSValueRef arguments[],
                JSValueRef* exception) {
    (void) function;
    (void) thisObject;
    (void) exception;

    JSStringRef js_result_string;
    GString *result = g_string_new("");

    if (argumentCount >= 1) {
        JSStringRef arg = JSValueToStringCopy(ctx, arguments[0], NULL);
        size_t arg_size = JSStringGetMaximumUTF8CStringSize(arg);
        char ctl_line[arg_size];
        JSStringGetUTF8CString(arg, ctl_line, arg_size);

        parse_cmd_line(ctl_line, result);

        JSStringRelease(arg);
    }
    js_result_string = JSStringCreateWithUTF8CString(result->str);

    g_string_free(result, TRUE);

    return JSValueMakeString(ctx, js_result_string);
}
