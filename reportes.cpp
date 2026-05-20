#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <iomanip>
#include "reportes.h"
#include "productos.h"
#include "ventas.h"

using namespace std;

struct ProductoVendido {
    int codigo;
    int totalVendido;
};


// ===============================
// MENU REPORTES
// ===============================
void menuReportes() {

    int opcion;

    do {

        cout << "\n===================================";
        cout << "\n           REPORTES";
        cout << "\n===================================";

        cout << "\n1. Productos menor stock";
        cout << "\n2. Productos mas vendidos";
        cout << "\n3. Ventas totales del dia";
        cout << "\n4. Ventas por mes";
        cout << "\n5. Ordenar precio ascendente";
        cout << "\n6. Ordenar precio descendente";
        cout << "\n7. Ordenar stock ascendente";
        cout << "\n8. Ordenar stock descendente";
        cout << "\n9. Ventas acumuladas por producto";
        cout << "\n10. Exportar reporte a TXT";
        cout << "\n11. Regresar";


        cout << "\n\nSeleccione una opcion: ";
        cin >> opcion;

        if(cin.fail()) {

            cout << "\nERROR: Debe ingresar numeros.\n";

            cin.clear();

            cin.ignore(1000, '\n');

            opcion = 0;

            continue;
        }

        switch(opcion) {

            case 1:
                productosMenorStock();
                break;

            case 2:
                productosMasVendidos();
                break;

            case 3:
                ventasTotalesDia();
                break;

            case 4:
                ventasPorMes();
                break;

            case 5:
                ordenarPrecioAsc();
                break;

            case 6:
                ordenarPrecioDesc();
                break;

           case 7:
                ordenarStockAsc();
                break;
           case 8:
                ordenarStockDesc();
                break;
           case 9:
                ventasAcumuladas();
                break;
           case 10:
                exportarReporteTXT();
                break;
           case 11:
                cout << "\nRegresando al menu principal...\n";
                break;
            default:
                cout << "\nOpcion invalida.\n";
        }

    } while(opcion != 8);
}



// PRODUCTOS MENOR STOCK
void productosMenorStock() {

    vector<Producto> productos;

    if(!cargarProductos(productos)) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    for(size_t i = 0; i < productos.size(); i++) {

        for(size_t j = 0; j < productos.size() - 1; j++) {

            if(productos[j].stock > productos[j + 1].stock) {
                Producto aux = productos[j];
                productos[j] = productos[j + 1];
                productos[j + 1] = aux;
            }
        }
    }

    cout << "\n========== PRODUCTOS CON MENOR STOCK ==========\n";

    for(const auto &p : productos) {

        if(p.activo) {

            cout << "\nCodigo: " << p.codigo;
            cout << "\nNombre: " << p.nombre;
            cout << "\nStock: " << p.stock;
            cout << "\nPrecio: Q " << p.precio;
            cout << "\n-----------------------------------";
        }
    }
}

// PRODUCTOS MAS VENDIDOS
void productosMasVendidos() {

    ifstream archivo("ventas.dat", ios::binary);

    if(!archivo) {

        cout << "\nNo existen ventas registradas.\n";
        return;
    }

    vector<ProductoVendido> vendidos;

    VentaHeader venta;

    while(archivo.read((char*)&venta, sizeof(venta))) {

        for(int i = 0; i < venta.detallesCount; i++) {

            DetalleVenta detalle;

            archivo.read((char*)&detalle, sizeof(detalle));

            bool encontrado = false;

            for(auto &v : vendidos) {

                if(v.codigo == detalle.codigo) {
                    v.totalVendido += detalle.cantidad;
                    encontrado = true;
                    break;
                }
            }

            if(!encontrado) {

                ProductoVendido nuevo;
                nuevo.codigo = detalle.codigo;
                nuevo.totalVendido = detalle.cantidad;
                vendidos.push_back(nuevo);
            }
        }
    }

    archivo.close();

    for(size_t i = 0; i < vendidos.size(); i++) {

        for(size_t j = 0; j < vendidos.size() - 1; j++) {

            if(vendidos[j].totalVendido < vendidos[j + 1].totalVendido) {

                ProductoVendido aux = vendidos[j];

                vendidos[j] = vendidos[j + 1];

                vendidos[j + 1] = aux;
            }
        }
    }

    vector<Producto> productos;

    cargarProductos(productos);

    cout << "\n========== PRODUCTOS MAS VENDIDOS ==========\n";

    for(const auto &v : vendidos) {

        int idx = buscarIndiceProducto(productos, v.codigo);

        if(idx != -1) {

            cout << "\nCodigo: " << productos[idx].codigo;
            cout << "\nNombre: " << productos[idx].nombre;
            cout << "\nCantidad Vendida: " << v.totalVendido;
            cout << "\n-----------------------------------";
        }
    }
}

// VENTAS TOTALES DEL DIA
void ventasTotalesDia() {

    ifstream archivo("ventas.dat", ios::binary);

    if(!archivo) {

        cout << "\nNo existen ventas registradas.\n";
        return;
    }

    // Obtener fecha actual
    time_t t = time(nullptr);

    tm* tm_ptr = localtime(&t);

    char fechaActual[11];

    snprintf(
        fechaActual,
        sizeof(fechaActual),
        "%04d-%02d-%02d",
        tm_ptr->tm_year + 1900,
        tm_ptr->tm_mon + 1,
        tm_ptr->tm_mday
    );

    float totalDia = 0;

    VentaHeader venta;

    while(archivo.read((char*)&venta, sizeof(venta))) {

        char fechaVenta[11];
        strncpy(fechaVenta, venta.fecha, 10);
        fechaVenta[10] = '\0';
        if(strcmp(fechaActual, fechaVenta) == 0) {
            totalDia += venta.total;
        }

        archivo.seekg(
            venta.detallesCount * sizeof(DetalleVenta),
            ios::cur
        );
    }

    archivo.close();

    cout << "\n========== VENTAS DEL DIA ==========\n";
    cout << "\nFecha: " << fechaActual;
    cout << "\nTotal vendido: Q " << totalDia << endl;
}



// VENTAS POR MES
void ventasPorMes() {

    float ventasMes[12] = {0};

    ifstream archivo("ventas.dat", ios::binary);

    if(!archivo) {

        cout << "\nNo existen ventas registradas.\n";
        return;
    }

    VentaHeader venta;

    while(archivo.read((char*)&venta, sizeof(venta))) {

        int anio;
        int mes;
        int dia;

        sscanf(venta.fecha, "%d-%d-%d", &anio, &mes, &dia);

        ventasMes[mes - 1] += venta.total;

        archivo.seekg(
            venta.detallesCount * sizeof(DetalleVenta),
            ios::cur
        );
    }

    archivo.close();

    string nombresMes[12] = {

        "Enero",
        "Febrero",
        "Marzo",
        "Abril",
        "Mayo",
        "Junio",
        "Julio",
        "Agosto",
        "Septiembre",
        "Octubre",
        "Noviembre",
        "Diciembre"
    };

    cout << "\n========== VENTAS POR MES ==========\n";

    for(int i = 0; i < 12; i++) {

        cout << "\n" << nombresMes[i]
             << ": Q " << ventasMes[i];
    }
}


// ORDENAR POR PRECIO ASCENDENTE
void ordenarPrecioAsc() {

    vector<Producto> productos;

    if(!cargarProductos(productos)) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    // Bubble Sort
    for(size_t i = 0; i < productos.size(); i++) {

        for(size_t j = 0; j < productos.size() - 1; j++) {

            if(productos[j].precio > productos[j + 1].precio) {

                Producto aux = productos[j];

                productos[j] = productos[j + 1];

                productos[j + 1] = aux;
            }
        }
    }

    cout << "\n========== PRECIOS ASCENDENTES ==========\n";

    for(const auto &p : productos) {

        if(p.activo) {

            cout << "\nCodigo: " << p.codigo;
            cout << "\nNombre: " << p.nombre;
            cout << "\nPrecio: Q " << p.precio;
            cout << "\nStock: " << p.stock;
            cout << "\n-----------------------------------";
        }
    }
}

// ORDENAR POR PRECIO DESCENDENTE
void ordenarPrecioDesc() {

    vector<Producto> productos;

    if(!cargarProductos(productos)) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    // Bubble Sort
    for(size_t i = 0; i < productos.size(); i++) {

        for(size_t j = 0; j < productos.size() - 1; j++) {

            if(productos[j].precio < productos[j + 1].precio) {

                Producto aux = productos[j];

                productos[j] = productos[j + 1];

                productos[j + 1] = aux;
            }
        }
    }

    cout << "\n========== PRECIOS DESCENDENTES ==========\n";

    for(const auto &p : productos) {

        if(p.activo) {

            cout << "\nCodigo: " << p.codigo;
            cout << "\nNombre: " << p.nombre;
            cout << "\nPrecio: Q " << p.precio;
            cout << "\nStock: " << p.stock;
            cout << "\n-----------------------------------";
        }
    }
}

// ORDENAR STOCK ASCENDENTE
void ordenarStockAsc() {

    vector<Producto> productos;

    if(!cargarProductos(productos)) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    for(size_t i = 0; i < productos.size(); i++) {

        for(size_t j = 0; j < productos.size() - 1; j++) {

            if(productos[j].stock > productos[j + 1].stock) {

                Producto aux = productos[j];

                productos[j] = productos[j + 1];

                productos[j + 1] = aux;
            }
        }
    }

    cout << "\n========== STOCK ASCENDENTE ==========\n";

    for(const auto &p : productos) {

        if(p.activo) {

            cout << "\nCodigo: " << p.codigo;
            cout << "\nNombre: " << p.nombre;
            cout << "\nStock: " << p.stock;
            cout << "\nPrecio: Q " << p.precio;
            cout << "\n-----------------------------------";
        }
    }
}


// ORDENAR STOCK DESCENDENTE
void ordenarStockDesc() {

    vector<Producto> productos;

    if(!cargarProductos(productos)) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    for(size_t i = 0; i < productos.size(); i++) {

        for(size_t j = 0; j < productos.size() - 1; j++) {

            if(productos[j].stock < productos[j + 1].stock) {

                Producto aux = productos[j];

                productos[j] = productos[j + 1];

                productos[j + 1] = aux;
            }
        }
    }

    cout << "\n========== STOCK DESCENDENTE ==========\n";

    for(const auto &p : productos) {

        if(p.activo) {

            cout << "\nCodigo: " << p.codigo;
            cout << "\nNombre: " << p.nombre;
            cout << "\nStock: " << p.stock;
            cout << "\nPrecio: Q " << p.precio;
            cout << "\n-----------------------------------";
        }
    }
}

// VENTAS ACUMULADAS

void ventasAcumuladas() {

    ifstream archivo("ventas.dat", ios::binary);

    if(!archivo) {

        cout << "\nNo existen ventas registradas.\n";
        return;
    }

    vector<ProductoVendido> vendidos;

    VentaHeader venta;

    while(archivo.read((char*)&venta, sizeof(venta))) {

        for(int i = 0; i < venta.detallesCount; i++) {

            DetalleVenta detalle;

            archivo.read((char*)&detalle, sizeof(detalle));

            bool encontrado = false;

            for(auto &v : vendidos) {

                if(v.codigo == detalle.codigo) {

                    v.totalVendido += detalle.cantidad;

                    encontrado = true;

                    break;
                }
            }

            if(!encontrado) {

                ProductoVendido nuevo;

                nuevo.codigo = detalle.codigo;
                nuevo.totalVendido = detalle.cantidad;
                vendidos.push_back(nuevo);
            }
        }
    }

    archivo.close();

    // Bubble Sort descendente
    for(size_t i = 0; i < vendidos.size(); i++) {

        for(size_t j = 0; j < vendidos.size() - 1; j++) {

            if(vendidos[j].totalVendido < vendidos[j + 1].totalVendido) {

                ProductoVendido aux = vendidos[j];

                vendidos[j] = vendidos[j + 1];

                vendidos[j + 1] = aux;
            }
        }
    }

    vector<Producto> productos;

    cargarProductos(productos);

    cout << "\n========== VENTAS ACUMULADAS ==========\n";

    for(const auto &v : vendidos) {

        int idx = buscarIndiceProducto(productos, v.codigo);

        if(idx != -1) {

            cout << "\nCodigo: " << productos[idx].codigo;
            cout << "\nNombre: " << productos[idx].nombre;
            cout << "\nCantidad Vendida: " << v.totalVendido;
            cout << "\n-----------------------------------";
        }
    }
}


// EXPORTAR REPORTE TXT

void exportarReporteTXT() {

    vector<Producto> productos;

    if(!cargarProductos(productos)) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    ofstream reporte("reporte.txt");

    if(!reporte) {

        cout << "\nError al crear reporte.\n";
        return;
    }

    reporte << "========== REPORTE PRODUCTOS ==========\n";

    for(const auto &p : productos) {

        if(p.activo) {

            reporte << "\nCodigo: " << p.codigo;
            reporte << "\nNombre: " << p.nombre;
            reporte << "\nCategoria: " << p.categoria;
            reporte << "\nStock: " << p.stock;
            reporte << "\nPrecio: Q " << p.precio;
            reporte << "\n-----------------------------------";
        }
    }

    reporte.close();

    cout << "\nReporte exportado correctamente.\n";
}
