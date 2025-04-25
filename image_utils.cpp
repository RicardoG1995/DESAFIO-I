#include "image_utils.h"
#include <QImage>
#include <QColor>
#include <fstream>
#include <iostream>
#include <sstream>

unsigned char* loadPixels(QString input, int &width, int &height) {
    QImage image(input);
    if (image.isNull()) {
        std::cerr << "Error: Could not load image " << input.toStdString() << std::endl;
        return nullptr;
    }

    width = image.width();
    height = image.height();
    unsigned char* pixels = new unsigned char[width * height * 3];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QColor color(image.pixel(x, y));
            int index = (y * width + x) * 3;
            pixels[index] = static_cast<unsigned char>(color.red());
            pixels[index + 1] = static_cast<unsigned char>(color.green());
            pixels[index + 2] = static_cast<unsigned char>(color.blue());
        }
    }

    return pixels;
}

bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida) {
    if (!pixelData || width <= 0 || height <= 0) {
        return false;
    }

    QImage image(width, height, QImage::Format_RGB888);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            image.setPixel(x, y, qRgb(pixelData[index],
                                    pixelData[index + 1],
                                    pixelData[index + 2]));
        }
    }

    return image.save(archivoSalida);
}

unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels) {
    std::ifstream file(nombreArchivo);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open masking file " << nombreArchivo << std::endl;
        return nullptr;
    }

    // Leer la primera línea (ignoraremos este número ya que puede ser incorrecto)
    int declared_pixels;
    if (!(file >> declared_pixels)) {
        std::cerr << "Error: Could not read number of pixels" << std::endl;
        file.close();
        return nullptr;
    }

    // Leer la siguiente línea para determinar el formato
    std::string line;
    std::getline(file, line); // Consumir el resto de la primera línea
    if (!std::getline(file, line)) {
        std::cerr << "Error: File is empty after pixel count" << std::endl;
        file.close();
        return nullptr;
    }

    // Contar cuántos números hay en la línea
    std::istringstream iss(line);
    int numeros = 0;
    int temp;
    while (iss >> temp) {
        numeros++;
    }

    // Contar el número real de líneas con datos
    file.clear();
    file.seekg(0);
    file >> declared_pixels; // Saltar la primera línea
    std::getline(file, line); // Consumir el resto de la primera línea

    int real_lines = 0;
    std::string temp_line;
    while (std::getline(file, temp_line)) {
        if (!temp_line.empty()) {
            real_lines++;
        }
    }

    // Volver al inicio después de la primera línea
    file.clear();
    file.seekg(0);
    file >> declared_pixels;
    std::getline(file, line); // Consumir el resto de la primera línea

    // Usar el número real de líneas como n_pixels
    n_pixels = real_lines;
    seed = 0;

    // Reservar memoria para los datos
    unsigned int* maskingData = new unsigned int[n_pixels * 3];
    int idx = 0;

    if (numeros == 2) {
        // Formato original (Caso 0 y 1): pares de valores
        unsigned int val1, val2;
        while (idx < n_pixels * 3 && file >> val1 >> val2) {
            maskingData[idx++] = val1;
            maskingData[idx++] = val1;
            maskingData[idx++] = val2;
        }
    } else if (numeros == 3) {
        // Formato nuevo (Caso 2): tripletas RGB
        unsigned int r, g, b;
        while (idx < n_pixels * 3 && std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::istringstream iss(line);
            if (iss >> r >> g >> b) {
                maskingData[idx++] = r;
                maskingData[idx++] = g;
                maskingData[idx++] = b;
            }
        }
    } else {
        std::cerr << "Error: Invalid file format (expected 2 or 3 values per line)" << std::endl;
        delete[] maskingData;
        file.close();
        return nullptr;
    }

    if (idx < n_pixels * 3) {
        std::cerr << "Error: Insufficient data in masking file" << std::endl;
        delete[] maskingData;
        file.close();
        return nullptr;
    }

    file.close();
    return maskingData;
} 