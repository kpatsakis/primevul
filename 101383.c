  void LaunchTestingApp(const std::string& extension_dirname) {
    base::FilePath data_dir;
    ASSERT_TRUE(PathService::Get(chrome::DIR_GEN_TEST_DATA, &data_dir));
    base::FilePath app_dir = data_dir.AppendASCII("ppapi")
                                     .AppendASCII("tests")
                                     .AppendASCII("extensions")
                                     .AppendASCII(extension_dirname)
                                     .AppendASCII(toolchain_);

    const extensions::Extension* extension = LoadExtension(app_dir);
    ASSERT_TRUE(extension);

    AppLaunchParams params(browser()->profile(), extension,
                           extensions::LAUNCH_CONTAINER_NONE, NEW_WINDOW,
                           extensions::SOURCE_TEST);
    params.command_line = *base::CommandLine::ForCurrentProcess();
    OpenApplication(params);
  }
