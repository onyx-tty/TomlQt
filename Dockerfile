FROM ubuntu:latest AS builder

RUN set -ex;                                                                                         \
    apt-get update;                                                                                  \
    apt-get install -y qt6-base-dev libtomlplusplus-dev libgtest-dev cmake git g++ ninja-build clang;

COPY . /app
WORKDIR /app

RUN set -ex;                     \
    cmake --preset Debug;        \
    cmake --build --preset Debug

RUN ctest --preset Debug --output-on-failure;
