#include "codegen/metadata.h"

std::vector<std::unique_ptr<MetadataAbstract>> MetadataLegalizer::metadataVec = {};

void  MetadataLegalizer::initDefaultVec(){
    // Integer
    metadataVec.push_back(std::make_unique<MetadataInteger>("stageWidth", 600));
    metadataVec.push_back(std::make_unique<MetadataInteger>("stageHeight", 500));
    metadataVec.push_back(std::make_unique<MetadataInteger>("roomSize", 4, 1, 8));
    metadataVec.push_back(std::make_unique<MetadataInteger>("lives", 3));
    metadataVec.push_back(std::make_unique<MetadataInteger>("maxAbilityLevel", 2, 0, 2));
    metadataVec.push_back(std::make_unique<MetadataInteger>("bornDuration", 300));
    metadataVec.push_back(std::make_unique<MetadataInteger>("bornLockDuration", 100));
    metadataVec.push_back(std::make_unique<MetadataInteger>("minPlayers", 1, 1, 8));
    // Boolean
    metadataVec.push_back(std::make_unique<MetadataBool>("supportSignin", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("mustLogin", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("allowGuest", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("supportMsgServer", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("runGame", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("campOpSkydow", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("campOpRoyal", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("campOpThird", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("setInitFocus", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("setBornDuration", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("nextGameEnabled", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("playDefaultMusic", true));
    metadataVec.push_back(std::make_unique<MetadataBool>("disableNextGameOnMissionComplete", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("useDefaultItems", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("defCarryItems", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("useDefaultCampLocs", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("useCustomWeapons", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("useCustomFarWeapons", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("useCustomItems", false));
    metadataVec.push_back(std::make_unique<MetadataBool>("allowGuest", true));
    // String
    metadataVec.push_back(std::make_unique<MetadataString>("schema", "", true));
    metadataVec.push_back(std::make_unique<MetadataString>("title", "", true));
    metadataVec.push_back(std::make_unique<MetadataString>("map", "", true));
    metadataVec.push_back(std::make_unique<MetadataString>("stageBackgroundColor", "#999999"));
    metadataVec.push_back(std::make_unique<MetadataString>("gamezoneCode", "mission"));
    // Enum
    // metadataVec.push_back(std::make_unique<MetadataEnum>("roomType", "close", {"close", "open"}));
    // metadataVec.push_back(std::make_unique<MetadataEnum>("stageAlignHorizontal", "center", {"left", "center", "right"}));
    // metadataVec.push_back(std::make_unique<MetadataEnum>("stageAlignVertical", "middle", {"top", "middle", "bottom"}));
    // metadataVec.push_back(std::make_unique<MetadataEnum>("cameraAfterOver", "free", {"free", "restrict", "none"}));
    // metadataVec.push_back(std::make_unique<MetadataEnum>("stageResolutionPolicy", "showAll",
    //              {"showAll", "exactFit", "noBorder", "fixedWidth",
    //               "fixedHeight", "origin"}),
    // metadataVec.push_back(std::make_unique<MetadataEnum>("gameStartFadein", "fadein",
    //              {"fadein", "fadeinL", "dark", "show"}));
    // List
    metadataVec.push_back(std::make_unique<MetadataList<Point>>("skydowLocs"));
    metadataVec.push_back(std::make_unique<MetadataList<Point>>("royalLocs"));
    metadataVec.push_back(std::make_unique<MetadataList<Point>>("thirdLocs"));
    metadataVec.push_back(std::make_unique<MetadataList<Point>>("initFocus"));
    metadataVec.push_back(std::make_unique<MetadataList<std::string>>("preloadSources"));
    metadataVec.push_back(std::make_unique<MetadataList<std::string>>("preloadResourcesExclude"));
    metadataVec.push_back(std::make_unique<MetadataList<std::string>>("carryItemCodes"));
    // std::make_unique<MetadataList<std::string>>("releaseCamp", {"third", "royal", "skydow"},
    //                           {"third", "royal", "skydow", "random"}),
    // std::make_unique<MetadataList<std::string>>("debugCamp", {"third", "royal", "skydow"},
    //                           {"third", "royal", "skydow", "random"}),
    // Unsupport
    metadataVec.push_back(std::make_unique<MetadataUnsupport>("customWeapons", ""));
    metadataVec.push_back(std::make_unique<MetadataUnsupport>("customFarWeapons", ""));
    metadataVec.push_back(std::make_unique<MetadataUnsupport>("customItems", ""));
}

void MetadataLegalizer::transform(){
    std::unordered_map<std::string, MetadataAbstract*> defaultMap;
    for(auto& defaultMeta: metadataVec)
        defaultMap[defaultMeta->key] = defaultMeta.get();
}