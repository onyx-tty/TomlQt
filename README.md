# TomlQt

Load Qt types directly from TOML configuration files.

## Features

**Conversions**: Extract and convert simple TOML types to Qt equivalents
- `value<QString>()`: String -> `std::optional<QString>`

**Mappings:**: Parse std::string into Qt semantic types
- `value<QSize>()`: Unsigned int array -> `std::optional<QSize>`
- `value<QSizePolicy>()`: String -> `std::optional<QSizePolicy>`
- `value<Qt::Alignment>()`: String array or string -> `std::optional<Qt::Alignment>`

**TOML++ support:** Forward native TOML++ types to the original value() implementation.
- `value<std::string>()` -> `std::optional<std::string>`
- `value<toml::date>()` -> `std::optional<toml::date>`
- `value<std::int64_t>()` -> `std::optional<std::int64_t>`

...and so on.

## Roadmap

- [ ] Support for string arrays in `value<QSizePolicy>()`, mimicking `value<Qt::Alignment>()`
- [X] `std::optional<TReturn>` getter `tomlqt::value<TReturn>`, similar to TOML++'s `node.value<T>()`
- [ ] `TReturn*` getter `tomlqt::as<TReturn>`, similar to TOML++'s `node.as_TYPE()`
- [ ] Constraints for templates

## Examples

### Conversion

``` C++
auto title = value<QString>(config["title"]); // QString("TomlQt")
auto title = value<std::string>(config["title"]); // Forwards TOML++ types to TOML++'s .value()
```

``` TOML
[config]
title = "TomlQt"
```

### Mapping

``` C++
auto size   = value<QSize>(config["size"]);           // QSize(1920, 1080)
auto policy = value<QSizePolicy>(config["policy"]);   // QSizePolicy(::Fixed | ::Fixed)
auto align1 = value<Qt::Alignment>(config["align1"]); // Qt::Alignment(::HCenter | ::Top)
auto align2 = value<Qt::Alignment>(config["align2"]); // Qt::Alignment(::HCenter | ::Top)
```

``` TOML
[config]
size   = [1920, 1080]
policy = "fixed"
align1 = "top"
align2 = ["hcenter", "top"]
```

### Building & Installation

#### Prerequisites

* `Qt6 Base`
* `Toml++`
* `GTest`
* `CMake`
* `Git`

##### Arch Linux

``` sh
sudo pacman -Sy qt6-base tomlplusplus gtest cmake git
```

##### Ubuntu

``` sh
sudo apt install qt6-base-dev libtomlplusplus-dev libgtest-dev cmake git
```

#### Build from source

``` sh
# Configure (static linking recommended)
cmake --preset Release

# Build
cmake --build --preset Release
```

**For dynamic linking**, pass `-DBUILD_SHARED_LIBS=ON` during configuration.
Make sure `/your/installation/path/lib` is in `$LD_LIBRARY_PATH`, so that shared libraries are found by the executable.

#### Install

CPack is not supported at present; installation is manual.

``` sh
cmake --install build/Release --prefix /your/installation/path
```

#### Test

``` sh
ctest --preset Release --output-on-failure
```

## Authors

* [onyx-tty](https://github.com/onyx-tty)

## License

This project is licensed under the MIT License — see the [LICENSE](./LICENSE) file for details
