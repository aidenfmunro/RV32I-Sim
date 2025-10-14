FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    wget \
    curl \
    python3 \
    cmake \
    gcc-riscv64-unknown-elf 

WORKDIR /workspace

CMD ["/bin/bash"]
