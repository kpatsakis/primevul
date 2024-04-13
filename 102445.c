bool ACodec::FlushingState::onOMXEvent(
        OMX_EVENTTYPE event, OMX_U32 data1, OMX_U32 data2) {
    ALOGV("[%s] FlushingState onOMXEvent(%u,%d)",
            mCodec->mComponentName.c_str(), event, (OMX_S32)data1);

 switch (event) {
 case OMX_EventCmdComplete:
 {
 if (data1 != (OMX_U32)OMX_CommandFlush) {
                ALOGE("unexpected EventCmdComplete %s(%d) data2:%d in FlushingState",
                        asString((OMX_COMMANDTYPE)data1), data1, data2);
                mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 return true;
 }

 if (data2 == kPortIndexInput || data2 == kPortIndexOutput) {
 if (mFlushComplete[data2]) {
                    ALOGW("Flush already completed for %s port",
                            data2 == kPortIndexInput ? "input" : "output");
 return true;
 }
                mFlushComplete[data2] = true;

 if (mFlushComplete[kPortIndexInput] && mFlushComplete[kPortIndexOutput]) {
                    changeStateIfWeOwnAllBuffers();
 }
 } else if (data2 == OMX_ALL) {
 if (!mFlushComplete[kPortIndexInput] || !mFlushComplete[kPortIndexOutput]) {
                    ALOGW("received flush complete event for OMX_ALL before ports have been"
 "flushed (%d/%d)",
                            mFlushComplete[kPortIndexInput], mFlushComplete[kPortIndexOutput]);
 return false;
 }

                changeStateIfWeOwnAllBuffers();
 } else {
                ALOGW("data2 not OMX_ALL but %u in EventCmdComplete CommandFlush", data2);
 }

 return true;
 }

 case OMX_EventPortSettingsChanged:
 {
            sp<AMessage> msg = new AMessage(kWhatOMXMessage, mCodec);
            msg->setInt32("type", omx_message::EVENT);
            msg->setInt32("node", mCodec->mNode);
            msg->setInt32("event", event);
            msg->setInt32("data1", data1);
            msg->setInt32("data2", data2);

            ALOGV("[%s] Deferring OMX_EventPortSettingsChanged",
                 mCodec->mComponentName.c_str());

            mCodec->deferMessage(msg);

 return true;
 }

 default:
 return BaseState::onOMXEvent(event, data1, data2);
 }

 return true;
}
