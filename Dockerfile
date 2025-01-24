FROM ubuntu:22.04

# Instalar herramientas necesarias
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    libboost-all-dev \
    libasio-dev

# Clonar Crow en el contenedor
RUN git clone https://github.com/CrowCpp/Crow.git /usr/local/src/crow

# Crear directorio de trabajo
WORKDIR /usr/src/app

# Crear carpeta build
RUN mkdir -p build

# Comando predeterminado
CMD ["/bin/bash"]
