#ifndef PRODUCTOS_H
#define PRODUCTOS_H

#include <vector>

using namespace std;

// STRUCT PRODUCTO
struct Producto {

    int codigo;
    char nombre[40];
    char categoria[30];
    int stock;
    float precio;
    bool activo;
};

// FUNCIONES PRODUCTOS
void registrarProducto();
void listarProductos();
void buscarProductoPorCodigo();
void buscarProductoPorNombre();
void actualizarStock();
void modificarPrecio();
void eliminarProducto();

// FUNCIONES AUXILIARES
bool cargarProductos(vector<Producto>& productos);
bool guardarProductos(const vector<Producto>& productos);
int buscarIndiceProducto(const vector<Producto>& productos, int codigo);

#endif