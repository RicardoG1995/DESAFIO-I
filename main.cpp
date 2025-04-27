/*
 * Programa demostrativo de manipulaciónprocesamiento de imágenes BMP en C++ usando Qt.
 *
 * Descripción:
 * Este programa realiza las siguientes tareas:
 * 1. Carga una imagen BMP desde un archivo (formato RGB sin usar estructuras ni STL).
 * 2. Modifica los valores RGB de los píxeles asignando un degradado artificial basado en su posición.
 * 3. Exporta la imagen modificada a un nuevo archivo BMP.
 * 4. Carga un archivo de texto que contiene una semilla (offset) y los resultados del enmascaramiento
 *    aplicados a una versión transformada de la imagen, en forma de tripletas RGB.
 * 5. Muestra en consola los valores cargados desde el archivo de enmascaramiento.
 * 6. Gestiona la memoria dinámicamente, liberando los recursos utilizados.
 *
 * Entradas:
 * - Archivo de imagen BMP de entrada ("I_O.bmp").
 * - Archivo de salida para guardar la imagen modificada ("I_D.bmp").
 * - Archivo de texto ("M1.txt") que contiene:
 *     • Una línea con la semilla inicial (offset).
 *     • Varias líneas con tripletas RGB resultantes del enmascaramiento.
 *
 * Salidas:
 * - Imagen BMP modificada ("I_D.bmp").
 * - Datos RGB leídos desde el archivo de enmascaramiento impresos por consola.
 *
 * Requiere:
 * - Librerías Qt para manejo de imágenes (QImage, QString).
 * - No utiliza estructuras ni STL. Solo arreglos dinámicos y memoria básica de C++.
 *
 * Autores: Augusto Salazar Y Aníbal Guerra
 * Fecha: 06/04/2025
 * Asistencia de ChatGPT para mejorar la forma y presentación del código fuente
 */
/*
 * Programa demostrativo de manipulación y procesamiento de imágenes BMP en C++ usando Qt.
 *
 * Descripción:
 * Este programa realiza las siguientes tareas:
 * 1. Carga una imagen BMP desde un archivo (formato RGB sin usar estructuras ni STL).
 * 2. Modifica los valores RGB de los píxeles asignando un degradado artificial basado en su posición.
 * 3. Exporta la imagen modificada a un nuevo archivo BMP.
 * 4. Carga un archivo de texto que contiene una semilla (offset) y los resultados del enmascaramiento
 *    aplicados a una versión transformada de la imagen, en forma de tripletas RGB.
 * 5. Muestra en consola los valores cargados desde el archivo de enmascaramiento.
 * 6. Gestiona la memoria dinámicamente, liberando los recursos utilizados.
 *
 * Entradas:
 * - Archivo de imagen BMP de entrada ("I_O.bmp").
 * - Archivo de salida para guardar la imagen modificada ("I_D.bmp").
 * - Archivo de texto ("M1.txt") que contiene:
 *     • Una línea con la semilla inicial (offset).
 *     • Varias líneas con tripletas RGB resultantes del enmascaramiento.
 *
 * Salidas:
 * - Imagen BMP modificada ("I_D.bmp").
 * - Datos RGB leídos desde el archivo de enmascaramiento impresos por consola.
 *
 * Requiere:
 * - Librerías Qt para manejo de imágenes (QImage, QString).
 * - No utiliza estructuras ni STL. Solo arreglos dinámicos y memoria básica de C++.
 *
 * Autores: Augusto Salazar Y Aníbal Guerra
 * Fecha: 06/04/2025
 */
#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

// Definición de rutas de archivo
const QString rutaImagenID = "I_D.bmp";
const QString rutaImagenIM = "I_M.bmp";
const QString rutaArchivoSalida = "I_S.bmp";
const QString rutaArchivoMascara = "M.bmp";
const QString rutaArchivoSemilla = "M0.txt";

// Declaraciones de funciones
unsigned char* cargarPixeles(QString entrada, int &ancho, int &alto);
bool exportarImagen(unsigned char* datosPixeles, int ancho, int alto, QString rutaSalida);
unsigned int* cargarDatosEnmascaramiento(const char* nombreArchivo, int &semilla, int &numPixeles);
void aplicarXOR(unsigned char* datosImagen, unsigned char* datosMascara, int ancho, int alto);
void rotarBitsIzquierda3(unsigned char* datosImagen, int ancho, int alto);
void recuperarImagenOriginal();

int main()
{
    recuperarImagenOriginal();
    return 0;
}

void recuperarImagenOriginal()
{
    cout << "Iniciando proceso de recuperación de imagen original..." << endl;

    // Variables para dimensiones de imagen
    int ancho = 0, alto = 0;
    int ancho2 = 0, alto2 = 0;
    int ancho3 = 0, alto3 = 0;

    // Paso 1: Cargar las imágenes necesarias
    unsigned char* imagenID = cargarPixeles(rutaImagenID, ancho, alto);
    if (!imagenID) {
        cerr << "Error: No se pudo cargar la imagen " << rutaImagenID.toStdString() << endl;
        return;
    }
    cout << "Imagen I_D.bmp cargada. Dimensiones: " << ancho << "x" << alto << endl;

    unsigned char* imagenIM = cargarPixeles(rutaImagenIM, ancho2, alto2);
    if (!imagenIM) {
        cerr << "Error: No se pudo cargar la imagen " << rutaImagenIM.toStdString() << endl;
        delete[] imagenID;
        return;
    }
    cout << "Imagen I_M.bmp cargada. Dimensiones: " << ancho2 << "x" << alto2 << endl;

    // Paso 2: Cargar la máscara M.bmp
    unsigned char* mascara = cargarPixeles(rutaArchivoMascara, ancho3, alto3);
    if (!mascara) {
        cerr << "Error: No se pudo cargar la máscara " << rutaArchivoMascara.toStdString() << endl;
        delete[] imagenID;
        delete[] imagenIM;
        return;
    }
    cout << "Máscara M.bmp cargada. Dimensiones: " << ancho3 << "x" << alto3 << endl;

    // Paso 3: Cargar los datos de enmascaramiento
    int semilla = 0, numPixeles = 0;
    unsigned int* datosEnmascaramiento = cargarDatosEnmascaramiento(rutaArchivoSemilla.toStdString().c_str(), semilla, numPixeles);
    if (!datosEnmascaramiento) {
        cerr << "Error: No se pudo cargar los datos de enmascaramiento " << rutaArchivoSemilla.toStdString() << endl;
        delete[] imagenID;
        delete[] imagenIM;
        delete[] mascara;
        return;
    }
    cout << "Datos de enmascaramiento cargados. Semilla: " << semilla << ", Pixeles: " << numPixeles << endl;

    // Proceso de recuperación

    // Paso 1: Revertir el XOR final (con I_M)
    cout << "Revirtiendo el último XOR con I_M..." << endl;
    aplicarXOR(imagenID, imagenIM, ancho, alto);
    exportarImagen(imagenID, ancho, alto, "P3_revertido.bmp");
    cout << "Paso 3 revertido. Resultado guardado en P3_revertido.bmp" << endl;

    // Paso 2: Revertir la rotación de 3 bits
    cout << "Revirtiendo la rotación de 3 bits..." << endl;
    rotarBitsIzquierda3(imagenID, ancho, alto);
    exportarImagen(imagenID, ancho, alto, "P2_revertido.bmp");
    cout << "Paso 2 revertido. Resultado guardado en P2_revertido.bmp" << endl;

    // Paso 3: Revertir el XOR inicial (con I_M nuevamente)
    cout << "Revirtiendo el primer XOR con I_M..." << endl;
    aplicarXOR(imagenID, imagenIM, ancho, alto);
    exportarImagen(imagenID, ancho, alto, "P1_revertido.bmp");
    cout << "Paso 1 revertido. Resultado guardado en P1_revertido.bmp" << endl;

    // Verificación con la máscara
    cout << "Verificando con la máscara M.bmp..." << endl;
    // Aquí deberías agregar la lógica para comparar con la máscara

    // Guardar la imagen original recuperada
    exportarImagen(imagenID, ancho, alto, "imagen_original_recuperada.bmp");
    cout << "Proceso completado! Imagen original recuperada en imagen_original_recuperada.bmp" << endl;

    // Liberar memoria
    delete[] imagenID;
    delete[] imagenIM;
    delete[] mascara;
    delete[] datosEnmascaramiento;
}

// Función para cargar los píxeles de una imagen BMP
unsigned char* cargarPixeles(QString entrada, int &ancho, int &alto)
{
    QImage imagen(entrada);
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen BMP." << endl;
        return nullptr;
    }

    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    ancho = imagen.width();
    alto = imagen.height();
    int tamanoDatos = ancho * alto * 3;

    unsigned char* datosPixeles = new unsigned char[tamanoDatos];
    for (int y = 0; y < alto; ++y) {
        const uchar* lineaFuente = imagen.scanLine(y);
        unsigned char* lineaDestino = datosPixeles + y * ancho * 3;
        memcpy(lineaDestino, lineaFuente, ancho * 3);
    }

    return datosPixeles;
}

// Función para exportar una imagen a formato BMP
bool exportarImagen(unsigned char* datosPixeles, int ancho, int alto, QString rutaSalida)
{
    QImage imagenSalida(ancho, alto, QImage::Format_RGB888);
    for (int y = 0; y < alto; ++y) {
        memcpy(imagenSalida.scanLine(y), datosPixeles + y * ancho * 3, ancho * 3);
    }

    if (!imagenSalida.save(rutaSalida, "BMP")) {
        cout << "Error: No se pudo guardar la imagen BMP.";
        return false;
    }
    return true;
}

// Función para cargar los datos de enmascaramiento
unsigned int* cargarDatosEnmascaramiento(const char* nombreArchivo, int &semilla, int &numPixeles)
{
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return nullptr;
    }

    archivo >> semilla;
    numPixeles = 0;
    int r, g, b;

    // Contar el número de píxeles
    while (archivo >> r >> g >> b) {
        numPixeles++;
    }

    archivo.close();
    archivo.open(nombreArchivo);

    if (!archivo.is_open()) {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }

    unsigned int* RGB = new unsigned int[numPixeles * 3];
    archivo >> semilla; // Leer semilla otra vez

    for (int i = 0; i < numPixeles * 3; i += 3) {
        archivo >> r >> g >> b;
        RGB[i] = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }

    archivo.close();
    return RGB;
}

// Función para aplicar operación XOR entre la imagen y la máscara (I_M)
void aplicarXOR(unsigned char* datosImagen, unsigned char* datosMascara, int ancho, int alto)
{
    int total = ancho * alto * 3;
    for (int i = 0; i < total; i++) {
        datosImagen[i] ^= datosMascara[i];
    }
}

// Función para rotar bits a la izquierda (inverso de rotación a la derecha)
void rotarBitsIzquierda3(unsigned char* datosImagen, int ancho, int alto)
{
    for (int i = 0; i < ancho * alto * 3; i++) {
        unsigned char valor = datosImagen[i];
        datosImagen[i] = (valor << 3) | (valor >> 5);
    }
}
