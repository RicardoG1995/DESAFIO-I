# Proyecto de Reconstrucción de Imágenes BMP Distorsionadas

## Descripción General
Este proyecto implementa un sistema para la reconstrucción de imágenes BMP que han sido distorsionadas mediante una serie de transformaciones. El sistema es capaz de detectar automáticamente las transformaciones aplicadas y revertirlas para recuperar la imagen original.

## Estructura del Proyecto

### Archivos Principales
- `main.cpp`: Implementación actual con detección automática de transformaciones
- `Inicial/MainInicial.cpp`: Versión inicial del proyecto con proceso manual
- `README.md`: Este archivo de documentación
- `Analisis.md`: Análisis detallado del problema y solución
- `evolucion.md`: Documentación de la evolución del proyecto
- `problemas.md`: Registro de problemas encontrados y soluciones
- `diagrama.md`: Diagramas de flujo y estructura
- `algoritmos.md`: Documentación detallada de algoritmos

### Archivos de Entrada/Salida
- `I_D.bmp`: Imagen distorsionada de entrada
- `I_M.bmp`: Imagen máscara para operaciones XOR
- `M.bmp`: Máscara de verificación
- `M*.txt`: Archivo de semilla y datos de enmascaramiento

## Características Principales
1. Detección automática de transformaciones aplicadas
2. Soporte para operaciones XOR con máscara
3. Rotación de bits (3 posiciones)
4. Verificación de resultados con máscara de control

## Requisitos del Sistema
- Qt Creator
- Compilador C++ compatible
- Sistema de archivos con permisos de lectura/escritura

## Uso del Programa
1. Colocar los archivos de entrada (I_D.bmp, I_M.bmp, M.bmp, M0.txt) en el directorio del proyecto
2. Compilar y ejecutar el programa
3. El sistema generará archivos intermedios y la imagen recuperada

## Documentación Adicional
Para información más detallada, consultar:
- [Análisis del Problema](Analisis.md)
- [Evolución del Proyecto](evolucion.md)
- [Problemas y Soluciones](problemas.md)
- [Diagramas](diagrama.md)
- [Algoritmos Implementados](algoritmos.md)