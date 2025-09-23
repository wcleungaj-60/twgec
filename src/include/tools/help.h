#include <string>

// TODO: Instead of hardcoding the help, it should be synchronized with the implementation
static const std::string helpMessage  = R"(twgec (Twilight game.events compiler) is a transpiler that can generate `game.events` file from `*.twge` file.

Usage: twgec [options] file
Options:
    --help          Display this information.
    --print-ast     Print the AST before each transformation.
    --print-token   Print the Token of the language.

Supported Global Metadata:
    __stageWidth__ : int
    __stageHeight__ : int
    __roomSize__ : int
    __lives__ : int
    __maxAbilityLevel__ : int
    __bornDuration__ : int
    __bornLockDuration__ : int
    __minPlayers__ : int
    __supportSignin__ : bool
    __mustLogin__ : bool
    __allowGuest__ : bool
    __supportMsgServer__ : bool
    __runGame__ : bool
    __campOpSkydow__ : bool
    __campOpRoyal__ : bool
    __campOpThird__ : bool
    __setInitFocus__ : bool
    __setBornDuration__ : bool
    __nextGameEnabled__ : bool
    __playDefaultMusic__ : bool
    __disableNextGameOnMissionComplete__ : bool
    __useDefaultItems__ : bool
    __defCarryItems__ : bool
    __useDefaultCampLocs__ : bool
    __useCustomWeapons__ : bool
    __useCustomFarWeapons__ : bool
    __useCustomItems__ : bool
    __title__ : string
    __map__ : string
    __schema__ : string
    __stageBackgroundColor__ : string
    __gamezoneCode__ : string
    __skydowLocs__ : list[Point]
    __royalLocs__ : list[Point]
    __thirdLocs__ : list[Point]

Supported Block Metadata:
    __delay__ : int
    __repeat__ : int
    __repeatInterval__ : int

Supported Actions:
    actorDisappear  -角色消失
        actorId: string, duration: int, delay: int
    actorTalk       角色說話
        cleanTalk: bool, text: string, duration: int, wait: bool, actorId: string
    addActor        +角色
        id: string, name: string, camp: string, teamId: int, weapon1: string, weapon2: string, localVarname: string, x: int, y: int, hp: int, range: int, role: string, externRole: string, patrol: list[Point]
    addMapSign      +告示牌
        text: string, buttonCode: string, buttonLabel: string, x: int, y: int, range: int, rotation: int
    addStuff        +武器道具
        code: string, item: string, refill: bool, refillInterval: int, x: int, y: int, range: int, rotation: int
    deltaHp         角色加減血
        actorCode: string, type: string, value: int, casterCode: string
    enblastEffect   光彈特效
        fromType: string, fromActor: string, toType: string, toAngle: int, damage: int, scale: int, speed: int
    missionComplete 任務完成
        camp: string
    longBo          +龍波
        actorCode: string
    print           控制台輸出
        type: string, text: string
    setGlobal       儲存全域變數
        key: string, type: string, value: int
    setObjectVar    儲存物件變數
        object: string, key: string, type: string, value: int
    wait            等待
        duration: int

Supported Checks:
    checkString     比對字串
        value: string, matchKind: string, str: string 

Supported Triggers:
    actorDead       角色死亡
        matchKind: string, actorId: string, varName: string, hitterVarName: string 
    actorFire       角色發動攻擊
        matchKind: string, actorId: string, varName: string
    clickButton     告示牌按鈕
        matchKind: string, actorId: string, varName: string, buttonId: string
    releasePower    發動技能
        matchKind: string, actorId: string, varName: string, ability: string, weapon: string
)";