FROM ubuntu:22.04

# Instalar herramientas necesarias
RUN apt-get update && apt-get install -qq \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libboost-all-dev \
    libasio-dev \
    libcurl4-openssl-dev \
    libgmock-dev \
    g++ \
    lcov 

# Clonar Crow en el contenedor
RUN git clone https://github.com/CrowCpp/Crow.git /usr/local/src/crow

# Instalar Google Test
RUN apt-get update && apt-get install -y \
    googletest \
    && apt-get clean

# Compilar Google Test
RUN cd /usr/src/googletest && \
    cmake . && \
    make && \
    cp -r googletest/include/gtest /usr/include && \
    cp lib/*.a /usr/lib

#ENV VARS
ENV CXXFLAGS="-g -O0 --coverage"
ENV LDFLAGS="--coverage"

# Crear directorio de trabajo
WORKDIR /usr/src/app

# Crear carpeta build
RUN mkdir -p build

# Comando predeterminado
CMD ["/bin/bash"]

# Generar cobertura
# lcov --capture --directory . --output-file coverage.info
# genhtml coverage.info --output-directory coverage-report