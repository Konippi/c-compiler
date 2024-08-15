FROM alpine:3.20

RUN apk update && \
    apk add --no-cache \
    gcc \
    make \
    git \
    binutils \
    libc-dev \