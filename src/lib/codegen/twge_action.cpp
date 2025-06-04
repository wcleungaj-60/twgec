#include "codegen/twge_action.h"
#include <fstream>

void twge::action::console(std::ofstream &of, std::unique_ptr<ActionNode> & action){
    of << inden(20) << "\"type\": \"Console\"," << std::endl;
    of << inden(20) << "\"data\": {" << std::endl;
    of << inden(24) << "\"logType\": \"" << action->identifier[1] << "\"," << std::endl;
    of << inden(24) << "\"text\": " << action->args[0] << "," << std::endl;
    of << inden(24) << "\"value\": \"\"" << std::endl;
    of << inden(20) << "}" << std::endl;
}