FROM ubuntu:latest

ARG WHOAMI

# Install necessary packages
RUN apt update && \
    apt install -y \
    gcc \
    make \
    git \
    binutils \
    libc6-dev \
    gdb \
    sudo

# Set up user
RUN adduser --disabled-password --gecos '' ${WHOAMI} && \
    echo '${WHOAMI} ALL=(root) NOPASSWD:ALL' > /etc/sudoers.d/${WHOAMI}

# Switch to created user
USER ${WHOAMI}

WORKDIR /home/${WHOAMI}
