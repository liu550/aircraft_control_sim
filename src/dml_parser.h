#ifndef AIRCRAFT_CONTROL_SIM_DML_PARSER_H_
#define AIRCRAFT_CONTROL_SIM_DML_PARSER_H_

#include "model.h"
#include <libxml/parser.h>
#include <libxml/tree.h>

class DMLParser {

public:
    void Parse(std::string fn);

private:

};

#endif