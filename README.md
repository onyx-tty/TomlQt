# TomlQt

Load Qt types directly from TOML configuration files.

## Features

**Conversions**: Extract and convert simple TOML types to Qt equivalents
- `tryGetQString()`: String -> `std::optional<QString>`

**Mappings:**: Parse std::string into Qt semantic types
- `tryGetQSize()`: Unsigned int array -> `std::optional<QSize>`
- `tryGetQSizePolicy()`: String -> `std::optional<QSizePolicy>`
- `tryGetQtAlignment()`: String array or string -> `std::optional<Qt::Alignment>`

## Roadmap

- [ ] Support for string arrays in `tryGetQSizePolicy()`, mimicking `tryGetQtAlignment()`
- [ ] `std::optional<T>` template overloads mimicking those in TOML++: `node.value<T>()`
- [ ] `T*` getters mimicking those in TOML++: `node.as_TYPE()`
- [ ] `T*` getter template overloads: `node.as<T>()`

## Examples

### Conversion

``` C++
auto title = tryGetQString(config["title"]); // QString("TomlQt")
```

``` TOML
[config]
title = "TomlQt"
```

### Mapping

``` C++
auto size   = tryGetQSize(config["size"]);         // QSize(1920, 1080)
auto policy = tryGetQSizePolicy(config["policy"]); // QSizePolicy(::Fixed | ::Fixed)
auto align1 = tryGetQtAlignment(config["align1"]); // Qt::Alignment(::HCenter | ::Top)
auto align2 = tryGetQtAlignment(config["align2"]); // Qt::Alignment(::HCenter | ::Top)
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

* `cmake`
* `ninja`
* `clang`
* `libtomlplusplus-dev`
* `qt6-base`
* `libgtest-dev`

##### Arch Linux

``` sh
sudo pacman -Sy tomlplusplus qt6-base cmake ninja clang gtest
```

##### Ubuntu

``` sh
sudo apt install libtomlplusplus-dev qt6-base-dev cmake ninja-build clang
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
