  MockWidgetInputHandler::MessageVector ExpectGestureScrollEndForWheelScrolling(
      bool is_last) {
    MockWidgetInputHandler::MessageVector events =
        GetAndResetDispatchedMessages();
    if (is_last) {
      EXPECT_EQ("GestureScrollEnd", GetMessageNames(events));
      return events;
    }
    EXPECT_EQ(0U, events.size());
    return events;
  }
