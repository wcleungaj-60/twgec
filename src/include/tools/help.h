#include <string>

static const std::string helpMessage  = R"(twgec (Twilight game.events compiler) is a transpiler that can generate `game.events` file from `*.twge` file.

Usage: twgec [options] file
Options:
    --help          Display this information.
    --print-ast     Print the AST before each transformation.
    --print-token   Print the Token of the language.

Supported Actions:
    actorDisappear  -角色消失
    actorTalk       角色說話
    addActor        +角色
    addMapSign      +告示牌
    addStuff        +武器道具
    deltaHp         角色加減血
    enblastEffect   光彈特效
    longBo          +龍波
    print           控制台輸出
    setGlobal       儲存全域變數
    setObjectVar    儲存物件變數
    wait            等待

Supported Checks:
    checkString     比對字串

Supported Triggers:
    actorDead       角色死亡
    actorFire       角色發動攻擊
    clickButton     告示牌按鈕
    releasePower    發動技能
)";