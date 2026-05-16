#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#include "ventas.h"
#include "productos.h"
#include "auxiliar.h"

using namespace std;

// CREAR VENTA

void crearVenta() {

    vector<Producto> productos;

    if(!cargarProductos(productos)) {

        cout << "\nNo existe informacion de productos.\n";
        return;
    }

    vector<DetalleVenta> detalles;

    int codigo;

    cout << "\n========== CREAR VENTA ==========\n";

    while(true) {

        cout << "\nCodigo producto (0 salir): ";
        cin >> codigo;

        if(codigo == 0) {
            break;
        }

        int idx = buscarIndiceProducto(productos, codigo);

        if(idx == -1) {

            cout << "Producto no encontrado.\n";
            continue;
        }

        int cantidad;

        cout << "Cantidad: ";
        cin >> cantidad;

        if(cantidad <= 0) {

            cout << "Cantidad invalida.\n";
            continue;
        }

        if(productos[idx].stock < cantidad) {

            cout << "Stock insuficiente.\n";
            continue;
        }

        bool repetido = false;

        for(auto &d : detalles) {

            if(d.codigo == codigo) {

                if(productos[idx].stock < d.cantidad + cantidad) {

                    cout << "Stock insuficiente.\n";
                    repetido = true;
                    break;
                }

                d.cantidad += cantidad;

                d.subtotal = d.cantidad * d.precioUnit;

                repetido = true;

                break;
            }
        }

        if(!repetido) {

            DetalleVenta d;

            d.codigo = codigo;
            d.cantidad = cantidad;
            d.precioUnit = productos[idx].precio;
            d.subtotal = d.cantidad * d.precioUnit;

            detalles.push_back(d);
        }

        cout << "Producto agregado.\n";
    }

    if(detalles.empty()) {

        cout << "\nVenta cancelada.\n";
        return;
    }

    int totalCantidad = 0;

    float subtotal = 0;

    for(const auto &d : detalles) {

        totalCantidad += d.cantidad;

        subtotal += d.subtotal;
    }

    float descuentoMonto = 0;

    if(subtotal > 500) {

        descuentoMonto = subtotal * 0.05f;
    }

    float descuentoBono = 0;

    if(totalCantidad % 5 == 0) {

        descuentoBono = subtotal * 0.02f;
    }

    float descuentoTotal = descuentoMonto + descuentoBono;

    float subtotalDescuento = subtotal - descuentoTotal;

    float iva = subtotalDescuento * 0.12f;

    float total = subtotalDescuento + iva;

    subtotal = round2(subtotal);
    descuentoTotal = round2(descuentoTotal);
    iva = round2(iva);
    total = round2(total);

    cout << "\n========== FACTURA ==========\n";

    for(const auto &d : detalles) {

        int idx = buscarIndiceProducto(productos, d.codigo);

        cout << "\nCodigo: " << d.codigo;

        cout << "\nNombre: " << productos[idx].nombre;

        cout << "\nCantidad: " << d.cantidad;

        cout << "\nPrecio: Q " << d.precioUnit;

        cout << "\nSubtotal: Q " << d.subtotal << "\n";
    }

    cout << "\nSubtotal: Q " << subtotal;
    cout << "\nDescuento: Q " << descuentoTotal;
    cout << "\nIVA: Q " << iva;
    cout << "\nTOTAL: Q " << total << endl;

    char confirmar;

    cout << "\nConfirmar venta? (s/n): ";
    cin >> confirmar;

    if(confirmar != 's' && confirmar != 'S') {

        cout << "\nVenta cancelada.\n";
        return;
    }

    int nextId = 1;

    ifstream inVentas("ventas.dat", ios::binary);

    if(inVentas) {

        while(!inVentas.eof()) {

            VentaHeader vh;

            if(!(inVentas.read((char*)&vh, sizeof(vh)))) {

                break;
            }

            inVentas.seekg(vh.detallesCount * sizeof(DetalleVenta), ios::cur);

            nextId = vh.id + 1;
        }

        inVentas.close();
    }

    time_t t = time(nullptr);

    tm* tm_ptr = localtime(&t);

    VentaHeader header;

    header.id = nextId;
    header.totalCantidad = totalCantidad;
    header.subtotal = subtotal;
    header.descuento = descuentoTotal;
    header.iva = iva;
    header.total = total;

    snprintf(
        header.fecha,
        sizeof(header.fecha),
        "%04d-%02d-%02d %02d:%02d",
        tm_ptr->tm_year + 1900,
        tm_ptr->tm_mon + 1,
        tm_ptr->tm_mday,
        tm_ptr->tm_hour,
        tm_ptr->tm_min
    );

    header.detallesCount = detalles.size();

    ofstream outVentas("ventas.dat", ios::binary | ios::app);

    if(!outVentas) {

        cout << "\nError al abrir ventas.dat\n";
        return;
    }

    outVentas.write((char*)&header, sizeof(header));

    for(const auto &d : detalles) {

        outVentas.write((char*)&d, sizeof(d));
    }

    outVentas.close();

    for(const auto &d : detalles) {

        int idx = buscarIndiceProducto(productos, d.codigo);

        if(idx != -1) {

            productos[idx].stock -= d.cantidad;
        }
    }

    guardarProductos(productos);

    cout << "\nVenta realizada correctamente.\n";
}