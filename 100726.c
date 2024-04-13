void ScriptController::executeScriptInIsolatedWorld(int worldID, const Vector<ScriptSourceCode>& sources, int extensionGroup, Vector<ScriptValue>* results)
{
    ASSERT(worldID > 0);

    v8::HandleScope handleScope;
    v8::Local<v8::Array> v8Results;
    {
        v8::HandleScope evaluateHandleScope;
        RefPtr<DOMWrapperWorld> world = DOMWrapperWorld::ensureIsolatedWorld(worldID, extensionGroup);
        V8WindowShell* isolatedWorldShell = windowShell(world.get());

        if (!isolatedWorldShell->isContextInitialized())
            return;

        v8::Local<v8::Context> context = isolatedWorldShell->context();
        v8::Context::Scope contextScope(context);
        v8::Local<v8::Array> resultArray = v8::Array::New(sources.size());

        for (size_t i = 0; i < sources.size(); ++i) {
            v8::Local<v8::Value> evaluationResult = compileAndRunScript(sources[i]);
            if (evaluationResult.IsEmpty())
                evaluationResult = v8::Local<v8::Value>::New(v8::Undefined());
            resultArray->Set(i, evaluationResult);
        }

        v8Results = evaluateHandleScope.Close(resultArray);
    }

    if (results && !v8Results.IsEmpty()) {
        for (size_t i = 0; i < v8Results->Length(); ++i)
            results->append(ScriptValue(v8Results->Get(i)));
    }
}
