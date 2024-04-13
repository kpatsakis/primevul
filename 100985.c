void GetPartOfMessageArguments(IPC::Message* message,
                               const base::DictionaryValue** out,
                               ExtensionMsg_MessageInvoke::Param* param) {
  ASSERT_EQ(ExtensionMsg_MessageInvoke::ID, message->type());
  ASSERT_TRUE(ExtensionMsg_MessageInvoke::Read(message, param));
  ASSERT_GE(std::get<3>(*param).GetSize(), 2u);
  const base::Value* value = NULL;
  ASSERT_TRUE(std::get<3>(*param).Get(1, &value));
  const base::ListValue* list = NULL;
  ASSERT_TRUE(value->GetAsList(&list));
  ASSERT_EQ(1u, list->GetSize());
  ASSERT_TRUE(list->GetDictionary(0, out));
}
