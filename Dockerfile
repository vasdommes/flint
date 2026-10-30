# syntax=docker/dockerfile:1

# latest alpine release
FROM alpine:3.24 AS build

# Argument for ./configure --host: a full triplet, e.g. x86_64-linux-musl.
# Default: generic CPU of the build platform, so that the image runs on any CPU.
# Without --host, configure optimizes for the build machine CPU (e.g. -march=tigerlake).
# An incomplete triplet (e.g. amd64 -> x86_64-pc-none) has no OS, so libtool builds only a static library.
ARG host

RUN apk add \
    autoconf \
    automake \
    cmake \
    g++ \
    gmp-dev \
    libtool \
    make \
    mpfr-dev \
    openblas \
    openblas-dev \
    python3

WORKDIR /usr/local/src/flint
# Build FLINT from current sources
COPY . .
RUN host=${host:-$(uname -m)-linux-musl} && \
    echo FLINT target host=$host && \
    ./bootstrap.sh && \
    ./configure --disable-static --with-blas --disable-pthread --prefix=/usr/local --host=$host && \
    make -j`nproc` && \
    make -j $(expr $(nproc) + 1) check  && \
    make -j`nproc` install

# Take only FLINT headers and binaries + load necessary dynamic libraries
FROM alpine:3.24 AS install
RUN apk add \
    gmp \
    libgmpxx \
    libstdc++ \
    mpfr \
    openblas
COPY --from=build /usr/local/bin /usr/local/bin
COPY --from=build /usr/local/include /usr/local/include
COPY --from=build /usr/local/lib /usr/local/lib
