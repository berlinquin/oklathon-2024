# Testing

I want to integrate GoogleTest into this project.

There's no clear documentation on the best practice for integrating GoogleTest into an existing Qt Project.

Starting with the Qt Creator [Test how-to](https://doc.qt.io/qtcreator/creator-how-tos.html#test), there are instructions to:
- [Add tests to existing test projects](https://doc.qt.io/qtcreator/creator-how-to-add-tests-to-projects.html)
- [Create Google tests](https://doc.qt.io/qtcreator/creator-how-to-create-google-tests.html)

These instructions don't say it directly, but suggest that the expected way to test Qt projects may be through a separate test project.

The design I would like:

- Create a separate `test` subdirectory in TrafficGram
- Put test source code, and separate CMakeLists.txt in `test`
- Include TrafficGram source as needed in test cases.
