# Algoritmos Implementados

## 1. Algoritmos de Transformación

### 1.1 Operación XOR (applyXOR)
```cpp
void applyXOR(unsigned char* imageData, unsigned char* maskData, int width, int height)
{
    int total = width * height * 3;
    for (int i = 0; i < total; i++) {
        imageData[i] ^= maskData[i];
    }
}
```
**Descripción**:
- Realiza una operación XOR bit a bit entre la imagen y la máscara
- Procesa cada componente RGB de cada píxel
- La operación es reversible: aplicarla dos veces restaura los valores originales

### 1.2 Rotación de Bits (rotateLeft3Bits/rotateRight3Bits)
```cpp
void rotateLeft3Bits(unsigned char* imageData, int width, int height)
{
    for (int i = 0; i < width * height * 3; i++) {
        unsigned char val = imageData[i];
        imageData[i] = (val << 3) | (val >> 5);
    }
}
```
**Descripción**:
- Rota los bits de cada byte 3 posiciones
- Preserva los bits que se desplazan fuera del byte
- La rotación izquierda es la inversa de la rotación derecha

## 2. Algoritmos de Carga y Procesamiento

### 2.1 Carga de Imágenes (loadPixels)
```cpp
unsigned char* loadPixels(QString input, int &width, int &height)
{
    QImage imagen(input);
    if (imagen.isNull()) return nullptr;
    
    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width = imagen.width();
    height = imagen.height();
    int dataSize = width * height * 3;
    
    unsigned char* pixelData = new unsigned char[dataSize];
    for (int y = 0; y < height; ++y) {
        memcpy(pixelData + y * width * 3, imagen.scanLine(y), width * 3);
    }
    return pixelData;
}
```
**Características**:
- Carga imágenes BMP en formato RGB888
- Manejo de errores y validación
- Asignación dinámica de memoria

### 2.2 Detección Automática (discoverTransformations)
```cpp
void discoverTransformations(unsigned char* id, unsigned char* im, int width, int height)
{
    unsigned char* trabajo = new unsigned char[width * height * 3];
    
    for (int t1 = 0; t1 < numTransforms; t1++) {
        for (int t2 = 0; t2 < numTransforms; t2++) {
            for (int t3 = 0; t3 < numTransforms; t3++) {
                memcpy(trabajo, id, width * height * 3);
                
                inverseTransforms[t3](trabajo, width, height);
                if (t3 == 0) applyXOR(trabajo, im, width, height);
                
                inverseTransforms[t2](trabajo, width, height);
                if (t2 == 0) applyXOR(trabajo, im, width, height);
                
                inverseTransforms[t1](trabajo, width, height);
                if (t1 == 0) applyXOR(trabajo, im, width, height);
                
                exportImage(trabajo, width, height,
                    QString("Paso1_%1_%2_%3.bmp").arg(t1).arg(t2).arg(t3));
            }
        }
    }
    delete[] trabajo;
}
```
**Características**:
- Prueba sistemática de combinaciones
- Generación de resultados intermedios
- Manejo eficiente de memoria

## 3. Algoritmos de Verificación

### 3.1 Exportación de Imágenes
```cpp
bool exportImage(unsigned char* pixelData, int width, int height, QString outputPath)
{
    QImage outputImage(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; ++y) {
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }
    return outputImage.save(outputPath, "BMP");
}
```
**Características**:
- Conversión de datos de píxeles a formato BMP
- Preservación del formato RGB888
- Manejo de errores en escritura

## 4. Optimizaciones Implementadas

### 4.1 Gestión de Memoria
- Uso de `memcpy` para copias eficientes
- Liberación sistemática de recursos
- Minimización de copias innecesarias

### 4.2 Procesamiento de Imágenes
- Operaciones a nivel de bits optimizadas
- Procesamiento secuencial de píxeles
- Reutilización de buffers

### 4.3 Validación y Control
- Verificación de dimensiones
- Manejo robusto de errores
- Generación de archivos intermedios

## 5. Consideraciones de Rendimiento

### 5.1 Complejidad Computacional
- Carga de imágenes: O(width * height)
- Transformaciones: O(width * height)
- Detección automática: O(n³ * width * height), donde n es el número de transformaciones

### 5.2 Uso de Memoria
- Buffer principal: width * height * 3 bytes
- Buffer de trabajo: width * height * 3 bytes
- Buffers temporales: según necesidad

### 5.3 Optimizaciones Futuras
- Paralelización de operaciones
- Procesamiento por bloques
- Caché de resultados intermedios