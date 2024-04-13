static int in_standby(struct audio_stream *stream)
{
 struct stream_in *in = (struct stream_in *)stream;
 struct audio_device *adev = in->dev;
 int status;
    ALOGV("%s: enter", __func__);
    pthread_mutex_lock(&adev->lock_inputs);
    status = in_standby_l(in);
    pthread_mutex_unlock(&adev->lock_inputs);
    ALOGV("%s: exit:  status(%d)", __func__, status);
 return status;
}
