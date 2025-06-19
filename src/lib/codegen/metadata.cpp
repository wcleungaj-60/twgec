#include "codegen/metadata.h"
#include "codegen_transformer.h"
#include "utils/utils.h"

DefaultMap MetadataLegalizer::defaultMap = DefaultMap({
    {metadata::stageWidth, {AST_INT, CODEGEN_INT, "600"}},
    {metadata::stageHeight, {AST_INT, CODEGEN_INT, "500"}},
    {metadata::roomSize, {AST_INT, CODEGEN_INT, "4"}},
    {metadata::lives, {AST_INT, CODEGEN_INT, "3"}},
    {metadata::maxAbilityLevel, {AST_INT, CODEGEN_INT, "2"}},
    {metadata::bornDuration, {AST_INT, CODEGEN_INT, "300"}},
    {metadata::bornLockDuration, {AST_INT, CODEGEN_INT, "100"}},
    {metadata::minPlayers, {AST_INT, CODEGEN_INT, "1"}},
    {metadata::supportSignin, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::mustLogin, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::allowGuest, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::supportMsgServer, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::runGame, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::campOpSkydow, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::campOpRoyal, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::campOpThird, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::setInitFocus, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::setBornDuration, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::nextGameEnabled, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::playDefaultMusic, {AST_BOOL, CODEGEN_BOOL, "true"}},
    {metadata::disableNextGameOnMissionComplete,
     {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::useDefaultItems, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::defCarryItems, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::useDefaultCampLocs, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::useCustomWeapons, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::useCustomFarWeapons, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::useCustomItems, {AST_BOOL, CODEGEN_BOOL, "false"}},
    {metadata::title, {AST_STRING, CODEGEN_STRING, "我的任務"}},
    {metadata::map,
     {AST_STRING, CODEGEN_STRING, "CG.TWEventsGameTemplate/gamemap.twmap"}},
    {metadata::schema,
     {AST_STRING, CODEGEN_STRING,
      "https://code.gamelet.com/gassets/schema/events/v1"}},
    {metadata::stageBackgroundColor, {AST_STRING, CODEGEN_STRING, "#999999"}},
    {metadata::gamezoneCode, {AST_STRING, CODEGEN_STRING, "mission"}},
    {metadata::skydowLocs, {AST_LIST_POINT, CODEGEN_LIST_POINT, "[]"}},
    {metadata::royalLocs, {AST_LIST_POINT, CODEGEN_LIST_POINT, "[]"}},
    {metadata::thirdLocs, {AST_LIST_POINT, CODEGEN_LIST_POINT, "[]"}},
    {metadata::preloadSources, {AST_LIST_STRING, CODEGEN_LIST_STRING, "[]"}},
    {metadata::preloadResourcesExclude,
     {AST_LIST_STRING, CODEGEN_LIST_STRING, "[]"}},
    {metadata::carryItemCodes, {AST_LIST_STRING, CODEGEN_LIST_STRING, "[]"}},
});

// Enum
// metadataVec.push_back(std::make_unique<MetadataEnum>("roomType", "close",
// {"close", "open"}));
// metadataVec.push_back(std::make_unique<MetadataEnum>("stageAlignHorizontal",
// "center", {"left", "center", "right"}));
// metadataVec.push_back(std::make_unique<MetadataEnum>("stageAlignVertical",
// "middle", {"top", "middle", "bottom"}));
// metadataVec.push_back(std::make_unique<MetadataEnum>("cameraAfterOver",
// "free", {"free", "restrict", "none"}));
// metadataVec.push_back(std::make_unique<MetadataEnum>("stageResolutionPolicy",
// "showAll",
//              {"showAll", "exactFit", "noBorder", "fixedWidth",
//               "fixedHeight", "origin"}),
// metadataVec.push_back(std::make_unique<MetadataEnum>("gameStartFadein",
// "fadein",
//              {"fadein", "fadeinL", "dark", "show"}));
// List
// std::make_unique<MetadataList<std::string>>("releaseCamp", {"third",
// "royal", "skydow"},
//                           {"third", "royal", "skydow", "random"}),
// std::make_unique<MetadataList<std::string>>("debugCamp", {"third", "royal",
// "skydow"},
//                           {"third", "royal", "skydow", "random"}),
// INIT_META_UNSUPPORT(metaMap, metadata::initFocus);
// INIT_META_UNSUPPORT(metaMap, metadata::customWeapons);
// INIT_META_UNSUPPORT(metaMap, metadata::customFarWeapons);
// INIT_META_UNSUPPORT(metaMap, metadata::customItems);