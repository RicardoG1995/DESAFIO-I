# Diagramas del Proyecto

## 1. Estructura General del Proyecto

```
DESAFIO-I/
├── main.cpp                 # Versión actual con detección automática
├── Inicial/
│   └── MainInicial.cpp     # Versión inicial del proyecto
├── I_D.bmp                 # Imagen distorsionada
├── I_M.bmp                 # Imagen máscara
├── M.bmp                   # Máscara de verificación
└── M0.txt                  # Archivo de semilla
```

## 2. Flujo de Proceso - Versión Inicial

```
[I_D.bmp] ──┐
            │
[I_M.bmp] ──┼─────> [Proceso Manual] ─────> [Transformaciones Inversas] ─────> [Imagen Original]
            │            │                           │
[M.bmp]  ───┤            │                           │
            │            v                           v
[M0.txt] ───┘     [Verificación] <───────── [Archivos Intermedios]
```

## 3. Flujo de Proceso - Versión Mejorada

```
                    ┌─────────────────────────────────────────┐
[I_D.bmp] ──┐       │     Sistema de Detección Automática     │
            │       │                                         │
[I_M.bmp] ──┼─────> │  ┌─────────┐    ┌──────────────────┐  │ ─────> [Imagen Original]
            │       │  │ Prueba   │ ─> │ Transformaciones │  │
[M.bmp]  ───┤       │  │ Combos  │    │    Inversas      │  │
            │       │  └─────────┘    └──────────────────┘  │
[M0.txt] ───┘       └─────────────────────────────────────────┘
```

## 4. Diagrama de Transformaciones

```
Transformación Original:
[Imagen] ──> [XOR con I_M] ──> [Rotación 3 bits] ──> [XOR con I_M] ──> [Imagen Distorsionada]

Proceso de Recuperación:
[I_D] <── [XOR con I_M] <── [Rotación -3 bits] <── [XOR con I_M] <── [Imagen Distorsionada]
```

## 5. Estructura de Clases y Funciones

```
Funciones Principales:
┌──────────────────────┐
│ recoverOriginalImage │
└──────────┬───────────┘
           │
           v
┌──────────────────────┐    ┌─────────────┐
│ discoverTransformations│ ──>│ TransformFunc│
└──────────────────────┘    └─────────────┘
           │
           v
┌──────────────────────┐
│    Transformaciones  │
├──────────────────────┤
│ - applyXOR          │
│ - rotateLeft3Bits   │
│ - rotateRight3Bits  │
└──────────────────────┘
```

## 6. Proceso de Verificación

```
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│ Cargar      │ ──> │ Aplicar      │ ──> │ Comparar    │
│ Archivos    │     │ Transformación│     │ con Máscara │
└─────────────┘     └──────────────┘     └─────────────┘
       │                   │                    │
       v                   v                    v
┌─────────────┐     ┌──────────────┐     ┌─────────────┐
│ Validar     │     │ Generar      │     │ Reportar    │
│ Dimensiones │     │ Intermedios  │     │ Resultados  │
└─────────────┘     └──────────────┘     └─────────────┘
```