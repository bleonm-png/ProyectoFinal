# Sistema de Ventas e Inventario

## Descripción

Sistema desarrollado en C++ para la gestión de productos, inventario y ventas. Permite registrar productos, controlar existencias, realizar ventas, generar reportes y exportar información a archivos de texto.

Este proyecto fue desarrollado aplicando programación modular, manejo de archivos binarios, estructuras de datos y control de errores.

---

## Funcionalidades

### Gestión de Productos

* Registrar productos.
* Buscar productos por código.
* Buscar productos por nombre.
* Modificar información de productos.
* Eliminar productos.
* Listar productos registrados.

### Gestión de Ventas

* Registrar ventas.
* Venta de múltiples productos por transacción.
* Validación automática de stock disponible.
* Actualización automática de inventario.
* Cálculo de subtotal, IVA y descuentos.

### Reportes

* Productos con menor stock.
* Productos más vendidos.
* Ventas totales del día.
* Ventas por mes.
* Estadísticas generales.
* Exportación de reportes a archivo TXT.

---

## Tecnologías Utilizadas

* C++
* Programación modular
* Archivos binarios (.dat)
* Estructuras
* Vectores
* Manejo de excepciones (try/catch)
* Archivos de texto (.txt)

---

## Estructura del Proyecto

ProyectoFinal

├── main.cpp

├── productos.cpp

├── productos.h

├── ventas.cpp

├── ventas.h

├── reportes.cpp

├── reportes.h

├── auxiliar.cpp

├── auxiliar.h

├── productos.dat

├── ventas.dat

└── reporte.txt

---

## Funciones Auxiliares

El proyecto incorpora funciones auxiliares reutilizables para mejorar la organización y mantenimiento del código, tales como:

* limpiarEntrada()
* mostrarEncabezado()
* round2()

---

## Validaciones Implementadas

* Validación de códigos duplicados.
* Validación de stock disponible.
* Validación de precios mayores a cero.
* Validación de cantidades válidas.
* Manejo de errores en apertura de archivos.
* Control de excepciones mediante try/catch.

---

## Autor

Bertha Jassel León Melchor
Carnet: 9941-25-15593
Ingeniería en Sistemas
Universidad Mariano Gálvez de Guatemala
