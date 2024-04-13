static void energy_info_recv_callback(bt_activity_energy_info *p_energy_info)
{
 if (!checkCallbackThread()) {
       ALOGE("Callback: '%s' is not called on the correct thread", __FUNCTION__);
 return;
 }

    callbackEnv->CallVoidMethod(sJniAdapterServiceObj, method_energyInfo, p_energy_info->status,
        p_energy_info->ctrl_state, p_energy_info->tx_time, p_energy_info->rx_time,
        p_energy_info->idle_time, p_energy_info->energy_used);

    checkAndClearExceptionFromCallback(callbackEnv, __FUNCTION__);
}
