ChromotingInstance::ChromotingInstance(PP_Instance pp_instance)
    : pp::Instance(pp_instance),
      initialized_(false),
      plugin_task_runner_(
          new PluginThreadTaskRunner(&plugin_thread_delegate_)),
      context_(plugin_task_runner_),
      input_tracker_(&mouse_input_filter_),
#if defined(OS_MACOSX)
      mac_key_event_processor_(&input_tracker_),
      key_mapper_(&mac_key_event_processor_),
#else
      key_mapper_(&input_tracker_),
#endif
      input_handler_(&key_mapper_),
      weak_factory_(ALLOW_THIS_IN_INITIALIZER_LIST(this)) {
  RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE | PP_INPUTEVENT_CLASS_WHEEL);
  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);

  RegisterLoggingInstance();

  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->SetInteger("apiVersion", kApiVersion);
  data->SetString("apiFeatures", kApiFeatures);
  data->SetInteger("apiMinVersion", kApiMinMessagingVersion);
  PostChromotingMessage("hello", data.Pass());
}
