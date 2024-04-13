void Part::slotTestingDone(KJob* job)
{
    if (job->error() && job->error() != KJob::KilledJobError) {
        KMessageBox::error(widget(), job->errorString());
    } else if (static_cast<TestJob*>(job)->testSucceeded()) {
        KMessageBox::information(widget(), i18n("The archive passed the integrity test."), i18n("Test Results"));
    } else {
        KMessageBox::error(widget(), i18n("The archive failed the integrity test."), i18n("Test Results"));
    }
}
