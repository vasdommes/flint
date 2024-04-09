# syntax=docker/dockerfile:1

# latest alpine release
FROM alpine:3.19 AS build

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
RUN ./bootstrap.sh && \
    ./configure --disable-static --enable-assert --with-blas  --disable-pthread --prefix=/usr/local && \
    make -j`nproc` && \
    make -j $(expr $(nproc) + 1) check  && \
    make -j`nproc` install

# Take only FLINT headers and binaries + load necessary dynamic libraries
FROM alpine:3.19 as install
RUN apk add \
    gmp \
    libgmpxx \
    libstdc++ \
    mpfr \
    openblas
COPY --from=build /usr/local/bin /usr/local/bin
COPY --from=build /usr/local/include /usr/local/include
COPY --from=build /usr/local/lib /usr/local/lib
