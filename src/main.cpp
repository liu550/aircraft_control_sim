#include <iostream>
#include "dml_parser.h"
using namespace std;

int main() {
    string fn = "data/Atmospheric_models/F16_package/F16_S119_source/F16_aero.dml";
    DMLParser dml_parser;
    AircraftModel model;

    dml_parser.Parse(fn);
}