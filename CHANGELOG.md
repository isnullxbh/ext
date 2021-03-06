# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.1.4] - 2021-06-23

### Added

- Add base class for library-defined exceptions (as part of [EXT-15](https://github.com/isnullxbh/ext/issues/15)).
- Reference ([EXT-15](https://github.com/isnullxbh/ext/issues/15)).
- Add rvalue ref-qualified overloads for value/error functions ([EXT-14](https://github.com/isnullxbh/ext/issues/14)).
- Add algorithms on tuples ([EXT-16](https://github.com/isnullxbh/ext/issues/16)).

### Changed

- Add support to switch compilation mode: header-only/compiled.

## [0.1.3] - 2021-05-27

### Added

- Define bind operator for the Result type: >>= ([EXT-5](https://github.com/isnullxbh/ext/issues/5)).
- Define map operator for the Result type: | ([EXT-6](https://github.com/isnullxbh/ext/issues/6)).
- Maybe ([EXT-4](https://github.com/isnullxbh/ext/issues/4)).
- None (as part of [EXT-4](https://github.com/isnullxbh/ext/issues/4)).
- Define bind operator for the Maybe type: >>= ([EXT-7](https://github.com/isnullxbh/ext/issues/7)).
- Define map operator for the Maybe type: | ([EXT-8](https://github.com/isnullxbh/ext/issues/8)).

## [0.1.2] - 2021-05-25

### Fixed:

- Add missing headers.

## [0.1.1] - 2021-05-24

### Added:

- Holder (as part of [EXT-1](https://github.com/isnullxbh/ext/issues/1)).
- Result ([EXT-1](https://github.com/isnullxbh/ext/issues/1)).

[Unreleased]: https://github.com/isnullxbh/ext/compare/v0.1.4...HEAD
[0.1.4]: https://github.com/isnullxbh/ext/compare/v0.1.3...v0.1.4
[0.1.3]: https://github.com/isnullxbh/ext/compare/v0.1.2...v0.1.3
[0.1.2]: https://github.com/isnullxbh/ext/compare/v0.1.1...v0.1.2
[0.1.1]: https://github.com/isnullxbh/ext/releases/v0.1.1
