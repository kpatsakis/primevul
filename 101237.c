    explicit DialogHandler(v8::Handle<v8::Value> dialogArguments, ScriptState* scriptState)
        : m_scriptState(scriptState)
        , m_dialogArguments(dialogArguments)
    {
    }
