#ifndef VENTAS_H
#define VENTAS_H

struct DetalleVenta {

    int codigo;
    int cantidad;
    float precioUnit;
    float subtotal;
};


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

void crearVenta();

#endif