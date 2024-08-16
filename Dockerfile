FROM ubuntu:latest

RUN apt update
RUN apt install -y gcc make git binutils libc6-dev gdb sudo
RUN adduser --disabled-password --gecos '' ${WHOAMI}
RUN echo '${WHOAMI} ALL=(root) NOPASSWD:ALL' > /etc/sudoers.d/${WHOAMI}

USER ${WHOAMI}

WORKDIR /home/${WHOAMI}
