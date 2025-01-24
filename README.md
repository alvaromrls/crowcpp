# Hash256 API

Una API RESTful simple para generar y validar hashes SHA-256 de textos y archivos. Esta API permite calcular el hash256 de cadenas de texto, comprobar la validez de un hash256 y trabajar con archivos.

## Funcionalidades

1. **Generar hash256 de un texto**: 
   - `GET /hash256/{text}`
   - Devuelve el hash256 de un texto proporcionado en la URL.

2. **Generar hash256 de un texto (POST)**: 
   - `POST /hash256`
   - Recibe un texto en el cuerpo de la solicitud y devuelve su hash256.

3. **Estado del servidor**: 
   - `GET /hash256/status`
   - Devuelve información sobre el estado de la API.

4. **Generar hash256 de un archivo**: 
   - `POST /hash256/file`
   - Recibe un archivo y devuelve su hash256.

5. **Validar hash256**: 
   - `POST /hash256/validate`
   - Recibe un texto y un hash256 y valida si el hash corresponde al texto.

6. **Obtener la versión de la API**: 
   - `GET /hash256/version`
   - Devuelve la versión de la API en uso.

7. **Información sobre la API**: 
   - `GET /hash256/about`
   - Devuelve información sobre la API, cómo funciona y sus características.

## Requisitos

- Docker (para la ejecución en contenedores)
- CMake 3.16 o superior
- Dependencias:
  - Crow (micro-framework C++ para crear APIs REST)
  - OpenSSL (para funciones de criptografía)
  - GTest (para pruebas unitarias)

## Instalación

### Usando Docker

Para iniciar el proyecto utilizando Docker, sigue estos pasos:

1. Clona el repositorio:

   ```bash
   git clone https://github.com/alvaromrls/crowcpp
   cd hash256-api
   ```

2. Construye y ejecuta el contenedor Docker:

   ```bash
   docker-compose up --build
   ```

   Esto construirá la imagen de Docker y pondrá en marcha el servidor en el puerto 8080.

3. Accede a la API desde tu navegador o usando herramientas como `curl`:

   ```bash
   curl http://localhost:8080/hash256/status
   ```

### Sin Docker

Si prefieres ejecutar el proyecto sin Docker:

1. Asegúrate de tener CMake y las dependencias necesarias instaladas.
2. Construye el proyecto:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Ejecuta el servidor:

   ```bash
   ./cppweb
   ```

   El servidor debería estar disponible en `http://localhost:8080`.

## Pruebas

Este proyecto incluye pruebas unitarias usando Google Test. Para ejecutar las pruebas:

1. Asegúrate de haber construido el proyecto correctamente con `make`.
2. Ejecuta las pruebas:

   ```bash
   make unit_tests
   ```



---
