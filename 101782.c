  void RunNavigationHintTest(
      const char* scope,
      content::StartServiceWorkerForNavigationHintResult expected_result,
      bool expected_started) {
    base::RunLoop run_loop;
    GetServiceWorkerContext()->StartServiceWorkerForNavigationHint(
        embedded_test_server()->GetURL(scope),
        base::BindOnce(&ExpectResultAndRun<
                           content::StartServiceWorkerForNavigationHintResult>,
                       expected_result, run_loop.QuitClosure()));
    run_loop.Run();
    if (expected_started) {
      histogram_tester_.ExpectBucketCount(
          "ServiceWorker.StartWorker.Purpose",
          27 /* ServiceWorkerMetrics::EventType::NAVIGATION_HINT  */, 1);
      histogram_tester_.ExpectBucketCount(
          "ServiceWorker.StartWorker.StatusByPurpose_NAVIGATION_HINT",
          0 /* SERVICE_WORKER_OK  */, 1);
    } else {
      histogram_tester_.ExpectTotalCount("ServiceWorker.StartWorker.Purpose",
                                         0);
      histogram_tester_.ExpectTotalCount(
          "ServiceWorker.StartWorker.StatusByPurpose_NAVIGATION_HINT", 0);
    }
    histogram_tester_.ExpectBucketCount(
        "ServiceWorker.StartForNavigationHint.Result",
        static_cast<int>(expected_result), 1);
  }
