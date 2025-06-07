#include "codegen/twge_action.h"
#include <fstream>
#include <iostream>

void twge::action::console(std::ofstream &of,
                           std::unique_ptr<ActionNode> &action) {
  if (action->identifier.size() != 2 ||
      (action->identifier[1] != "log" && action->identifier[1] != "error")) {
    std::cerr << "Expecting console.log or console.error for the action at "
              << action->loc << "\n";
    return;
  }
  if (action->positional_args.size() != 1) {
    std::cerr << "Expecting only one positional arg for console action at "
              << action->loc << "\n";
    return;
  }
  if (action->named_args.size() != 0) {
    std::cerr << "Expecting no named arg for console action at " << action->loc
              << "\n";
    return;
  }
  of << inden(20) << "\"type\": \"Console\"," << std::endl;
  of << inden(20) << "\"data\": {" << std::endl;
  of << inden(24) << "\"logType\": \"" << action->identifier[1] << "\","
     << std::endl;
  of << inden(24) << "\"text\": " << action->positional_args[0].get()->value
     << "," << std::endl;
  of << inden(24) << "\"value\": \"\"" << std::endl;
  of << inden(20) << "}" << std::endl;
}

void twge::action::addActor(std::ofstream &of,
                            std::unique_ptr<ActionNode> &action) {
  of << inden(20) << "\"type\": \"AddActor\"," << std::endl;
  of << inden(20) << "\"data\": {" << std::endl;
  of << inden(24) << "\"actorCode\": \"ai*\"," << std::endl;
  of << inden(24) << "\"name\": \"\"," << std::endl;
  of << inden(24) << "\"role\": {" << std::endl;
  of << inden(28) << "\"dr\": 0" << std::endl;
  of << inden(24) << "}," << std::endl;
  of << inden(24) << "\"actorType\": \"defaultType\"," << std::endl;
  of << inden(24) << "\"weapon0\": {" << std::endl;
  of << inden(28) << "\"w0Type\": \"default\"" << std::endl;
  of << inden(24) << "}," << std::endl;
  of << inden(24) << "\"weapon1\": {" << std::endl;
  of << inden(28) << "\"w1Type\": \"default\"" << std::endl;
  of << inden(24) << "}," << std::endl;
  of << inden(24) << "\"camp\": \"skydow\"," << std::endl;
  of << inden(24) << "\"group\": \"0\"," << std::endl;
  of << inden(24) << "\"location\": {" << std::endl;
  of << inden(28) << "\"x\": \"0\"," << std::endl;
  of << inden(28) << "\"y\": \"0\"," << std::endl;
  of << inden(28) << "\"range\": \"0\"" << std::endl;
  of << inden(24) << "}," << std::endl;
  of << inden(24) << "\"shiftX\": 0," << std::endl;
  of << inden(24) << "\"shiftY\": 0," << std::endl;
  of << inden(24) << "\"spawnLoc\": true," << std::endl;
  of << inden(24) << "\"rotation\": \"0\"," << std::endl;
  of << inden(24) << "\"idleRotate\": true," << std::endl;
  of << inden(24) << "\"maxhp\": \"100\"," << std::endl;
  of << inden(24) << "\"manaPower\": \"0\"," << std::endl;
  of << inden(24) << "\"lives\": \"1\"," << std::endl;
  of << inden(24) << "\"preferAbilityLevel\": \"smart\"," << std::endl;
  of << inden(24) << "\"maxAbilityLevel\": 1," << std::endl;
  of << inden(24) << "\"weight\": \"4\"," << std::endl;
  of << inden(24) << "\"strength\": \"1\"," << std::endl;
  of << inden(24) << "\"vision\": \"300\"," << std::endl;
  of << inden(24) << "\"range\": \"10000\"," << std::endl;
  of << inden(24) << "\"score\": \"10\"," << std::endl;
  of << inden(24) << "\"bloodType\": \"default\"," << std::endl;
  of << inden(24) << "\"unbrokenArmor\": true," << std::endl;
  of << inden(24) << "\"tornadoRes\": \"none\"," << std::endl;
  of << inden(24) << "\"distractWhenHit\": true," << std::endl;
  of << inden(24) << "\"thinkInterval\": 60," << std::endl;
  of << inden(24) << "\"farAutoLevel\": \"0\"," << std::endl;
  of << inden(24) << "\"bornDelayDuration\": \"0\"," << std::endl;
  of << inden(24) << "\"bornDuration\": \"1000\"," << std::endl;
  of << inden(24) << "\"bornLockDuration\": \"1000\"," << std::endl;
  of << inden(24) << "\"bornEffect\": \"none\"," << std::endl;
  of << inden(24) << "\"bornAnim\": \"fadein\"," << std::endl;
  of << inden(24) << "\"localVarname\": \"actor\"," << std::endl;
  of << inden(24) << "\"globalVarname\": \"\"" << std::endl;
  of << inden(20) << "}" << std::endl;
}