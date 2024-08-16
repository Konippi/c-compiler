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
RUN adduser --disabled-password --gecos "" ${WHOAMI} && \
    echo "${WHOAMI} ALL=(root) NOPASSWD:ALL" > /etc/sudoers

# Switch to created user
USER ${WHOAMI}

WORKDIR /home/${WHOAMI}

ENV HOME_DIR /home/${WHOAMI}

# Set up environment
RUN echo "export PS1='[\u@\h \w]\$ '" >> ${HOME_DIR}/.bashrc
RUN echo "source ${HOME_DIR}/.bashrc" >> ${HOME_DIR}/.bash_profile
