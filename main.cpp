//==============================
// BERTHA JASSEL LEON MELCHOR
//=============================
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "productos.h"
#include "ventas.h"
#include "reportes.h"

using namespace std;

void menu();

int main() {

    try {

        menu();

    } catch(const exception& e) {

        cout << "\n===================================";
        cout << "\n ERROR DEL SISTEMA";
        cout << "\n===================================";
        cout << "\n" << e.what() << endl;
    }

    return 0;
}

// MENU
void menu() {

    int opcion;

    do {

        cout << fixed << setprecision(2);

        cout << "\n===================================";
        cout << "\n SISTEMA DE VENTAS E INVENTARIO";
        cout << "\n===================================";
        cout << "\n Autor: Bertha Jassel Leon Melchor";
        cout << "\n Carnet: 9941.25.15593";
        cout << "\n===================================";
        cout << "\n1. Registrar producto";
        cout << "\n2. Listar productos";
        cout << "\n3. Buscar producto por codigo";
        cout << "\n4. Buscar producto por nombre";
        cout << "\n5. Gestion inventario";
        cout << "\n6. Crear venta";
        cout << "\n7. Reportes";
        cout << "\n8. Salir";

        cout << "\n\nSeleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {

            case 1:
                registrarProducto();
                break;

            case 2:
                listarProductos();
                break;

            case 3:
                buscarProductoPorCodigo();
                break;

            case 4:
                buscarProductoPorNombre();
                break;

            case 5:
                menuInventario();
                break;

            case 6:
                crearVenta();
                break;

            case 7:
                menuReportes();
                break;

            case 8:
                cout << "\nPrograma finalizado.\n";
                break;

            default:
                cout << "\nOpcion invalida.\n";
        }

    } while(opcion != 8);
}
