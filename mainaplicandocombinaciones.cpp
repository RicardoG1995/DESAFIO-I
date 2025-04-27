/*
 * Programa para descubrir transformaciones de imágenes automáticamente
 */
#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>

using namespace std;

// Definición de rutas de archivo
const QString imagenID = "I_D.bmp";
const QString imagenIM = "I_M.bmp";
const QString archivoSalida = "I_S.bmp";
const QString maskFile = "M.bmp";
const QString seedMaskFile = "M0.txt";

// Declaraciones de funciones
unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);
void applyXOR(unsigned char* imageData, unsigned char* maskData, int width, int height);
void rotateLeft3Bits(unsigned char* imageData, int width, int height);
void rotateRight3Bits(unsigned char* imageData, int width, int height);
void discoverTransformations(unsigned char* id, unsigned char* im, int width, int height);
void recoverOriginalImage();

// Tipos de funciones de transformación
typedef void (*TransformFunc)(unsigned char*, int, int);

// Lista de transformaciones posibles
TransformFunc inverseTransforms[] = {
    [](unsigned char* img, int w, int h) { /* XOR se aplica igual */ },
    rotateLeft3Bits
};

const char* transformNames[] = {
    "XOR con I_M",
    "Rotacion de 3 bits a la derecha (revierte izquierda)"
};

const int numTransforms = 2;

int main()
{
    recoverOriginalImage();
    return 0;
}

void recoverOriginalImage()
{
    cout << "Iniciando proceso de recuperacion de imagen original..." << endl;

    // Variables para dimensiones de imagen
    int width = 0, height = 0;
    int width2 = 0, height2 = 0;

    // Paso 1: Cargar las imágenes necesarias
    unsigned char* id = loadPixels(imagenID, width, height);
    if (!id) {
        cerr << "Error: No se pudo cargar la imagen " << imagenID.toStdString() << endl;
        return;
    }
    cout << "Imagen I_D.bmp cargada. Dimensiones: " << width << "x" << height << endl;

    unsigned char* im = loadPixels(imagenIM, width2, height2);
    if (!im) {
        cerr << "Error: No se pudo cargar la imagen " << imagenIM.toStdString() << endl;
        delete[] id;
        return;
    }
    cout << "Imagen I_M.bmp cargada. Dimensiones: " << width2 << "x" << height2 << endl;

    // Verificar dimensiones
    if (width != width2 || height != height2) {
        cerr << "Error: Las dimensiones de I_D e I_M no coinciden" << endl;
        delete[] id;
        delete[] im;
        return;
    }

    // Paso 2: Descubrir transformaciones automáticamente
    discoverTransformations(id, im, width, height);

    // Liberar memoria
    delete[] id;
    delete[] im;
}

void discoverTransformations(unsigned char* id, unsigned char* im, int width, int height)
{
    cout << "\nIniciando descubrimiento automatico de transformaciones..." << endl;

    // Crear copia de trabajo
    unsigned char* trabajo = new unsigned char[width * height * 3];

    // Probar todas las combinaciones posibles de transformaciones
    for (int t1 = 0; t1 < numTransforms; t1++) {
        for (int t2 = 0; t2 < numTransforms; t2++) {
            for (int t3 = 0; t3 < numTransforms; t3++) {
                // Copiar imagen transformada
                memcpy(trabajo, id, width * height * 3);

                // Aplicar transformaciones en orden inverso
                inverseTransforms[t3](trabajo, width, height);
                if (t3 == 0) applyXOR(trabajo, im, width, height);

                inverseTransforms[t2](trabajo, width, height);
                if (t2 == 0) applyXOR(trabajo, im, width, height);

                inverseTransforms[t1](trabajo, width, height);
                if (t1 == 0) applyXOR(trabajo, im, width, height);

                // Guardar resultado para inspección
                QString nombreArchivo = QString("Paso1_%1_%2_%3.bmp").arg(t1).arg(t2).arg(t3);
                exportImage(trabajo, width, height, nombreArchivo);

                cout << "Probada combinacion: "
                     << transformNames[t1] << " -> "
                     << transformNames[t2] << " -> "
                     << transformNames[t3]
                     << " | Guardado en " << nombreArchivo.toStdString() << endl;
            }
        }
    }

    cout << "\nProceso completado. Revise los archivos generados para ver los resultados.\n";
    cout << "Los nombres de archivo indican el orden de transformaciones aplicadas (1_2_3).\n";
    cout << "0 = XOR con I_M, 1 = Rotacion de 3 bits derecha\n";

    delete[] trabajo;
}

// Función para cargar los píxeles de una imagen BMP
unsigned char* loadPixels(QString input, int &width, int &height)
{
    QImage imagen(input);
    if (imagen.isNull()) {
        cerr << "Error: No se pudo cargar la imagen " << input.toStdString() << endl;
        return nullptr;
    }

    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width = imagen.width();
    height = imagen.height();
    int dataSize = width * height * 3;

    unsigned char* pixelData = new unsigned char[dataSize];
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = imagen.scanLine(y);
        unsigned char* dstLine = pixelData + y * width * 3;
        memcpy(dstLine, srcLine, width * 3);
    }

    return pixelData;
}

// Función para exportar una imagen a formato BMP
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida)
{
    QImage outputImage(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; ++y) {
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }

    if (!outputImage.save(archivoSalida, "BMP")) {
        cerr << "Error: No se pudo guardar la imagen " << archivoSalida.toStdString() << endl;
        return false;
    }
    return true;
}

// Función para aplicar operación XOR entre la imagen y la máscara (I_M)
void applyXOR(unsigned char* imageData, unsigned char* maskData, int width, int height)
{
    int total = width * height * 3;
    for (int i = 0; i < total; i++) {
        imageData[i] ^= maskData[i];
    }
}

// Función para rotar bits a la izquierda (inverso de rotación a la derecha)
void rotateLeft3Bits(unsigned char* imageData, int width, int height)
{
    for (int i = 0; i < width * height * 3; i++) {
        unsigned char val = imageData[i];
        imageData[i] = (val << 3) | (val >> 5);
    }
}

// Función para rotar bits a la derecha
void rotateRight3Bits(unsigned char* imageData, int width, int height)
{
    for (int i = 0; i < width * height * 3; i++) {
        unsigned char val = imageData[i];
        imageData[i] = (val >> 3) | (val << 5);
    }
}

