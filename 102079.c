bool DoesFormContainAmbiguousOrEmptyNames(
    const PasswordFormFillData& fill_data) {
  return (fill_data.username_field.name == fill_data.password_field.name) ||
         (fill_data.password_field.name ==
              base::ASCIIToUTF16(kDummyPasswordField) &&
          (!FillDataContainsFillableUsername(fill_data) ||
           fill_data.username_field.name ==
               base::ASCIIToUTF16(kDummyUsernameField)));
}
