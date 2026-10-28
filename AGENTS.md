# Repository Guidelines

## Project Structure & Module Organization

This workspace is currently empty and has not been initialized as a Git repository. No source layout, language, framework, or asset directories are established yet. Update this guide when the initial implementation introduces them.

For new code, group related functionality into clearly named modules. Keep tests alongside their modules or in a dedicated `tests/` directory, following the chosen framework’s conventions. Store static assets separately from executable source, and document the resulting layout in `README.md`.

## Build, Test, and Development Commands

No build, test, or local development commands are configured. When adding tooling, expose repeatable commands through the ecosystem’s standard manifest or task runner and document them in `README.md`.

Include commands for installing dependencies, running locally, building, testing, and linting where applicable. State any required runtime versions. Do not document commands as supported until their configuration exists and they have been verified.

## Coding Style & Naming Conventions

No formatter, linter, or indentation standard is established. Use the selected language’s conventional formatting, then commit shared formatter and linter configuration. Follow those settings consistently rather than introducing competing styles.

Choose descriptive module, function, and variable names. Keep functions focused, and avoid unrelated formatting changes in functional patches.

## Testing Guidelines

No testing framework or coverage threshold exists yet. Add tests with the first testable functionality, covering expected behavior, boundary cases, and failure paths. Use the framework’s standard test filenames and descriptive names that explain the behavior under test. Document the exact test command when configured.

## Commit & Pull Request Guidelines

There is no Git history from which to infer commit conventions. Use concise, imperative commit subjects, such as `Add initial project structure`, and keep each commit focused.

Pull requests should describe the change, its purpose, and validation performed. Link relevant issues and include screenshots for visible UI changes. Identify any missing checks explicitly.

## Security & Configuration

Keep credentials and local configuration out of version control. Provide placeholder configuration examples and add appropriate ignore rules when initializing the project.
