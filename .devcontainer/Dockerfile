ARG VARIANT=debian-12
FROM --platform=linux/amd64 mcr.microsoft.com/devcontainers/base:${VARIANT}
USER root

# Install needed packages. Use a separate RUN statement to add your own dependencies.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install build-essential cmake cppcheck valgrind clang lldb llvm gdb \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Setup ENV vars for vcpkg
ENV VCPKG_ROOT=/usr/local/vcpkg \
    VCPKG_DOWNLOADS=/usr/local/vcpkg-downloads
ENV PATH="${PATH}:${VCPKG_ROOT}"

ARG USERNAME=vscode


# [Optional] Uncomment this section to install additional OS packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends gcc g++ make


RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends cmake clang

RUN wget https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/xc8-v2.50-full-install-linux-x64-installer.run

# Set the working directory to /app
# WORKDIR /app

RUN chmod +x xc8-v2.50-full-install-linux-x64-installer.run
RUN ./xc8-v2.50-full-install-linux-x64-installer.run --mode unattended  --unattendedmodeui none  --netservername host

RUN apt-get clean &&  apt-get update --allow-insecure-repositories && sudo apt-get install -y --allow-unauthenticated ninja-build

RUN apt-get clean &&  apt-get update --allow-insecure-repositories && sudo apt-get install -y --allow-unauthenticated clangd-16
RUN sudo update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-16 100