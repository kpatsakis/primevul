bool ScriptController::canAccessFromCurrentOrigin(Frame *frame)
{
    return !v8::Context::InContext() || BindingSecurity::shouldAllowAccessToFrame(frame);
}
