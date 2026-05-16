#ifndef VENTAS_H
#define VENTAS_H

// STRUCT DETALLE VENTA
struct DetalleVenta {

    int codigo;
    int cantidad;
    float precioUnit;
    float subtotal;
};

// STRUCT VENTA
struct VentaHeader {

    int id;
    int totalCantidad;
    float subtotal;
    float descuento;
    float iva;
    float total;
    char fecha[20];
    int detallesCount;
};

// FUNCIONES
void crearVenta();

#endif