bool ShouldQuicMigrateSessionsEarlyV2(
    const VariationParameters& quic_trial_params) {
  return base::LowerCaseEqualsASCII(
      GetVariationParam(quic_trial_params, "migrate_sessions_early_v2"),
      "true");
}
