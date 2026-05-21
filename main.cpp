//==============================
// BERTHA JASSEL LEON MELCHOR
//=============================

#include <iostream>
#include <iomanip>
#include "productos.h"
#include "ventas.h"
#include "reportes.h"

using namespace std;

void menu();

int main() {
    menu();
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
