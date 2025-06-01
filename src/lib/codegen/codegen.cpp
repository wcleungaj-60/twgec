#include "codegen.h"
#include <fstream>
#include <iostream>

/** Helper Function */
std::string inden(int value) { return std::string(value, ' '); }

std::string getMetaString(
    std::unordered_map<std::string, std::unique_ptr<MetadataAbstract>> metaMap,
    std::string key) {
  return (static_cast<MetadataString *>(metaMap[key].get()))->value;
}

void CodeGen::codegen(std::string filepath) {
  std::ofstream outputFile(filepath);
  if (!outputFile)
    return;
  codegenModuleNode(outputFile);
  outputFile.close();
}

void CodeGen::codegenMetaData(std::ofstream &of) {
  of << inden(4) << "\"$schema\": " << metadataLegalizer.getStr("schema") << ",";
  of << R"(
    "config": {
        "stage": {
            "width": 600,
            "height": 500,
            "backgroundColor": "#999999",
            "resolutionPolicy": "showAll",
            "alignHorizontal": "center",
            "alignVertical": "middle"
        },
        "preload": {
            "sources": [],
            "resourcesExclude": []
        },
        "configs": {
            "TwilightWarsConfig": {
  )";
  of << inden(12) << "\"title\": " << metadataLegalizer.getStr( "title") << ",";
  of << R"(
                "serverConfig": {
                    "minPlayers": 1,
                    "supportSignin": true,
                    "mustLogin": true,
                    "allowGuest": true,
                    "supportMsgServer": true,
                    "gamezoneCode": "mission",
                    "roomType": "close",
                    "roomSize": 4
                },
                "runGame": true,
                "gameStartFadein": "fadein",
                "lives": 3,
                "debugCamp": "ask",
                "releaseCamp": "ask",
                "setInitFocus": false,
                "initFocus": {
                    "x": "0",
                    "y": "0"
                },
                "campOptions": {
                    "campOpSkydow": true,
                    "campOpRoyal": true,
                    "campOpThird": true
                },
  )";
  of << inden(12) << "\"map\": " << metadataLegalizer.getStr("map") << ",";
  of << R"(
                "maxAbilityLevel": 2,
                "nextGameEnabled": true,
                "disableNextGameOnMissionComplete": false,
                "playDefaultMusic": true,
                "cameraAfterOver": "restrict",
                "useDefaultItems": false,
                "useDefaultCampLocs": true,
                "skydowLocs": [],
                "royalLocs": [],
                "thirdLocs": [],
                "useCustomWeapons": true,
                "customWeapons": [],
                "useCustomItems": false,
                "customItems": [],
                "enabled": true,
                "@use": 0
            }
        }
    },
  )";
}

void CodeGen::codegenModuleNode(std::ofstream &of) {
  of << "{" << std::endl;
  codegenMetaData(of);
  of << "    \"events\": []" << std::endl;
  of << "}" << std::endl;
}