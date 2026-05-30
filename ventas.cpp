#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstring>

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

    cout << "\n========== CREAR VENTA ==========\n";

    while(true) {

        int opcionBusqueda;

        cout << "\n1. Buscar por codigo";
        cout << "\n2. Buscar por nombre";
        cout << "\n3. Finalizar venta";

        cout << "\nSeleccione opcion: ";
        cin >> opcionBusqueda;

        if(cin.fail()) {

            cout << "\nERROR: Debe ingresar numeros.\n";

            limpiarEntrada();

            continue;
        }

        int idx = -1;

        // BUSCAR POR CODIGO
        if(opcionBusqueda == 1) {

            int codigo;

            cout << "\nCodigo producto: ";
            cin >> codigo;

            idx = buscarIndiceProducto(productos, codigo);
        }

        // BUSCAR POR NOMBRE
        else if(opcionBusqueda == 2) {

            char nombreBuscado[40];

            limpiarEntrada();

            cout << "\nNombre producto: ";

            cin.getline(nombreBuscado, 40);

            for(size_t i = 0; i < productos.size(); i++) {

                if(
                    strstr(productos[i].nombre, nombreBuscado) != nullptr
                    && productos[i].activo
                ) {

                    idx = i;

                    break;
                }
            }
        }

        // FINALIZAR VENTA
        else if(opcionBusqueda == 3) {

            break;
        }

        else {

            cout << "\nOpcion invalida.\n";

            continue;
        }

        // VALIDAR PRODUCTO
        if(idx == -1) {

            cout << "\nProducto no encontrado.\n";

            continue;
        }

        int cantidad;

        cout << "\nCantidad: ";
        cin >> cantidad;

        if(cin.fail()) {

            cout << "\nERROR: Debe ingresar numeros.\n";

            limpiarEntrada();

            continue;
        }

        if(cantidad <= 0) {

            cout << "\nCantidad invalida.\n";

            continue;
        }

        if(productos[idx].stock < cantidad) {

            cout << "\nStock insuficiente.\n";

            continue;
        }

        bool repetido = false;

        for(auto &d : detalles) {

            if(d.codigo == productos[idx].codigo) {

                if(productos[idx].stock < d.cantidad + cantidad) {

                    cout << "\nStock insuficiente.\n";

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

            d.codigo = productos[idx].codigo;

            strcpy(
                d.nombreProducto,
                productos[idx].nombre
            );

            d.cantidad = cantidad;

            d.precioUnit = productos[idx].precio;

            d.subtotal = d.cantidad * d.precioUnit;

            detalles.push_back(d);
        }

        cout << "\nProducto agregado correctamente.\n";
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

        cout << "\nCodigo: " << d.codigo;

        cout << "\nNombre: " << d.nombreProducto;

        cout << "\nCantidad: " << d.cantidad;

        cout << "\nPrecio Unitario: Q " << d.precioUnit;

        cout << "\nSubtotal: Q " << d.subtotal;

        cout << "\n-----------------------------------";
    }

    cout << "\n\nSubtotal: Q " << subtotal;
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

        while(true) {

            VentaHeader vh;

            if(!(inVentas.read((char*)&vh, sizeof(vh)))) {

                break;
            }

            inVentas.seekg(
                vh.detallesCount * sizeof(DetalleVenta),
                ios::cur
            );

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

    ofstream outVentas(
        "ventas.dat",
        ios::binary | ios::app
    );

    if(!outVentas) {

        cout << "\nError al abrir ventas.dat\n";
        return;
    }

    outVentas.write((char*)&header, sizeof(header));

    for(const auto &d : detalles) {

        outVentas.write((char*)&d, sizeof(d));
    }

    outVentas.close();

    // ACTUALIZAR STOCK
    for(const auto &d : detalles) {

        int idx = buscarIndiceProducto(
            productos,
            d.codigo
        );

        if(idx != -1) {

            productos[idx].stock -= d.cantidad;
        }
    }

    guardarProductos(productos);

    cout << "\nVenta realizada correctamente.\n";
}