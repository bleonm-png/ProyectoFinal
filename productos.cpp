#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include "productos.h"

using namespace std;

// CARGAR PRODUCTOS

bool cargarProductos(vector<Producto>& productos) {
    productos.clear();
    Producto p;
    ifstream archivo("productos.dat", ios::binary);

    if(!archivo) {
        return false;
    }
    while(archivo.read((char*)&p, sizeof(p))) {

        productos.push_back(p);
    }
    archivo.close();
    return true;
}

// GUARDAR PRODUCTOS

bool guardarProductos(const vector<Producto>& productos) {

    ofstream archivo("productos.dat", ios::binary | ios::trunc);

    if(!archivo) {
        return false;
    }

    for(const auto& p : productos) {

        archivo.write((char*)&p, sizeof(p));
    }

    archivo.close();

    return true;
}


// BUSCAR INDICE PRODUCTO

int buscarIndiceProducto(const vector<Producto>& productos, int codigo) {

    for(size_t i = 0; i < productos.size(); i++) {

        if(productos[i].codigo == codigo && productos[i].activo) {

            return i;
        }
    }

    return -1;
}

// REGISTRAR PRODUCTO

void registrarProducto() {

    try {

        Producto producto;

        vector<Producto> productos;

        cargarProductos(productos);

        cout << "\n========== REGISTRO ==========\n";

        cout << "Codigo: ";
        cin >> producto.codigo;

        if(cin.fail()) {
            throw "Codigo invalido.";
        }

        for(auto &p : productos) {

            if(p.codigo == producto.codigo && p.activo) {

                throw "El codigo ya existe.";
            }
        }

        cin.ignore(1000, '\n');

        cout << "Nombre: ";
        cin.getline(producto.nombre, 40);

        cout << "Categoria: ";
        cin.getline(producto.categoria, 30);

        cout << "Stock: ";
        cin >> producto.stock;

        if(producto.stock < 0) {

            throw "El stock no puede ser negativo.";
        }

        cout << "Precio: ";
        cin >> producto.precio;

        if(producto.precio <= 0) {

            throw "Precio invalido.";
        }

        producto.activo = true;

        ofstream archivo("productos.dat", ios::binary | ios::app);

        if(!archivo) {

            throw "Error al abrir archivo.";
        }

        archivo.write((char*)&producto, sizeof(producto));

        archivo.close();

        cout << "\nProducto registrado correctamente.\n";
    }

    catch(const char* mensaje) {

        cout << "\nERROR: " << mensaje << "\n";

        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// LISTAR PRODUCTOS

void listarProductos() {

    Producto p;

    ifstream archivo("productos.dat", ios::binary);

    if(!archivo) {

        cout << "\nNo existe informacion almacenada.\n";
        return;
    }

    cout << "\n========== LISTADO ==========\n";

    while(archivo.read((char*)&p, sizeof(p))) {

        if(p.activo) {

            cout << "\n---------------------------";

            cout << "\nCodigo: " << p.codigo;
            cout << "\nNombre: " << p.nombre;
            cout << "\nCategoria: " << p.categoria;
            cout << "\nStock: " << p.stock << " Unidades";
            cout << "\nPrecio: Q " << p.precio << "\n";
        }
    }

    archivo.close();
}

// BUSCAR POR CODIGO

void buscarProductoPorCodigo() {

    int codigoBuscado;

    cout << "\nIngrese el codigo: ";
    cin >> codigoBuscado;

    if(cin.fail()) {

        cout << "\nERROR: Debe ingresar un numero.\n";

        cin.clear();
        cin.ignore(1000, '\n');

        return;
    }

    Producto p;

    ifstream archivo("productos.dat", ios::binary);

    if(!archivo) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    bool encontrado = false;

    while(archivo.read((char*)&p, sizeof(p))) {

        if(p.codigo == codigoBuscado && p.activo) {

            cout << "\nProducto encontrado:\n";

            cout << "Codigo: " << p.codigo << endl;
            cout << "Nombre: " << p.nombre << endl;
            cout << "Categoria: " << p.categoria << endl;
            cout << "Stock: " << p.stock << endl;
            cout << "Precio: Q " << p.precio << endl;

            encontrado = true;

            break;
        }
    }

    archivo.close();

    if(!encontrado) {

        cout << "\nProducto no encontrado.\n";
    }
}

// BUSCAR POR NOMBRE

void buscarProductoPorNombre() {

    char nombreBuscado[40];

    cin.ignore();

    cout << "\nIngrese el nombre: ";

    cin.getline(nombreBuscado, 40);
    if(strlen(nombreBuscado) == 0) {

        cout << "\nERROR: Debe ingresar un nombre.\n";

        return;
    }

    Producto p;

    ifstream archivo("productos.dat", ios::binary);

    if(!archivo) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    bool encontrado = false;

    while(archivo.read((char*)&p, sizeof(p))) {

        if(strstr(p.nombre, nombreBuscado) != nullptr && p.activo) {

            cout << "\nProducto encontrado:\n";

            cout << "Codigo: " << p.codigo << endl;
            cout << "Nombre: " << p.nombre << endl;
            cout << "Categoria: " << p.categoria << endl;
            cout << "Stock: " << p.stock << endl;
            cout << "Precio: Q " << p.precio << endl;

            encontrado = true;
        }
    }

    archivo.close();

    if(!encontrado) {

        cout << "\nProducto no encontrado.\n";
    }
}

// ACTUALIZAR STOCK

void actualizarStock() {

    try {

        int codigoBuscado;
        int nuevoStock;

        cout << "\nIngrese codigo del producto: ";
        cin >> codigoBuscado;

        cout << "Nuevo stock: ";
        cin >> nuevoStock;

        if(nuevoStock < 0) {

            throw "Stock invalido.";
        }

        vector<Producto> productos;

        if(!cargarProductos(productos)) {

            throw "No existe informacion.";
        }

        bool encontrado = false;

        for(auto &p : productos) {

            if(p.codigo == codigoBuscado && p.activo) {

                p.stock = nuevoStock;

                encontrado = true;
            }
        }

        if(!encontrado) {

            throw "Producto no encontrado.";
        }

        guardarProductos(productos);

        cout << "\nStock actualizado correctamente.\n";
    }

    catch(const char* mensaje) {

        cout << "\nERROR: " << mensaje << "\n";
    }
}

// MODIFICAR PRECIO

void modificarPrecio() {

    try {

        int codigoBuscado;
        float nuevoPrecio;

        cout << "\nIngrese codigo del producto: ";
        cin >> codigoBuscado;

        cout << "Nuevo precio: ";
        cin >> nuevoPrecio;

        if(nuevoPrecio <= 0) {

            throw "Precio invalido.";
        }

        vector<Producto> productos;

        if(!cargarProductos(productos)) {

            throw "No existe informacion.";
        }

        bool encontrado = false;

        for(auto &p : productos) {

            if(p.codigo == codigoBuscado && p.activo) {

                p.precio = nuevoPrecio;

                encontrado = true;
            }
        }

        if(!encontrado) {

            throw "Producto no encontrado.";
        }

        guardarProductos(productos);

        cout << "\nPrecio actualizado correctamente.\n";
    }

    catch(const char* mensaje) {

        cout << "\nERROR: " << mensaje << "\n";
    }
}

// ELIMINAR PRODUCTO

void eliminarProducto() {

    int codigoBuscado;

    cout << "\nIngrese codigo del producto: ";
    cin >> codigoBuscado;

    vector<Producto> productos;

    if(!cargarProductos(productos)) {

        cout << "\nNo existe informacion.\n";
        return;
    }

    bool encontrado = false;

    for(auto &p : productos) {

        if(p.codigo == codigoBuscado && p.activo) {

            p.activo = false;

            encontrado = true;
        }
    }

    if(encontrado) {

        guardarProductos(productos);

        cout << "\nProducto desactivado correctamente.\n";
    }
    else {

        cout << "\nProducto no encontrado.\n";
    }
}
// REINICIAR INVENTARIO

void reiniciarInventario() {

    char confirmar;

    cout << "\nSeguro que desea reiniciar inventario? (s/n): ";
    cin >> confirmar;

    if(confirmar == 's' || confirmar == 'S') {

        ofstream archivo("productos.dat", ios::binary | ios::trunc);

        archivo.close();

        cout << "\nInventario reiniciado correctamente.\n";
    }
    else {

        cout << "\nOperacion cancelada.\n";
    }
}

// MENU INVENTARIO
void menuInventario() {

    int opcion;

    do {

        cout << "\n========== GESTION INVENTARIO ==========";

        cout << "\n1. Actualizar stock";
        cout << "\n2. Modificar precio";
        cout << "\n3. Desactivar producto";
        cout << "\n4. Reiniciar inventario";
        cout << "\n5. Regresar";

        cout << "\n\nSeleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {

            case 1:
                actualizarStock();
                break;

            case 2:
                modificarPrecio();
                break;

            case 3:
                eliminarProducto();
                break;

            case 4:
                reiniciarInventario();
                break;

            case 5:
                break;

            default:
                cout << "\nOpcion invalida.\n";
        }

    } while(opcion != 5);
}