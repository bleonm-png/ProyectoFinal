#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include "auxiliar.h"

using namespace std;

void limpiarEntrada() {

    cin.clear();

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

float round2(float val) {

    return round(val * 100.0f) / 100.0f;
}


