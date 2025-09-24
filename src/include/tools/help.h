#include <string>

// TODO: Instead of hardcoding the help, it should be synchronized with the implementation
static const std::string helpMessage  = R"(twgec (Twilight game.events compiler) is a transpiler that can generate `game.events` file from `*.twge` file.

Usage: twgec [options] file
Options:
    --help          Display this information.
    --print-ast     Print the AST before each transformation.
    --print-token   Print the Token of the language.

Builtin Struct:
    Point
        x: int, y: int
    ActorMatch
        id: string, matchKind: string

Supported Global Metadata:
    __stageWidth__ : int|string
    __stageHeight__ : int|string
    __roomSize__ : int|string
    __lives__ : int|string
    __maxAbilityLevel__ : int|string
    __bornDuration__ : int|string
    __bornLockDuration__ : int|string
    __minPlayers__ : int|string
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
    __delay__ : int|string
    __repeat__ : int|string
    __repeatInterval__ : int|string

Supported Actions:
    actorDisappear  -角色消失
        actorId: string, duration: int|string, delay: int|string
    actorFollow     跟隨人物
        actorId: string, type: string, targetId: string
    actorTalk       角色說話
        cleanTalk: bool, text: string, duration: int|string, wait: bool, actorId: string
    addActor        +角色
        id: string, name: string, camp: string, teamId: int|string, weapon1: string, weapon2: string, localVarname: string, x: int|string, y: int|string, hp: int|string, range: int|string, role: string, externRole: string, patrol: list[Point], strength: int|string
    addMapSign      +告示牌
        text: string, buttonCode: string, buttonLabel: string, x: int|string, y: int|string, range: int|string, rotation: int|string
    addStuff        +武器道具
        code: string, item: string, refill: bool, refillInterval: int|string, x: int|string, y: int|string, range: int|string, rotation: int|string
    deltaHp         角色加減血
        actorCode: string, type: string, value: int|string, casterCode: string
    enblastEffect   光彈特效
        fromType: string, fromActor: string, toType: string, toAngle: int|string, damage: int|string, scale: int|string, speed: int|string
    missionComplete 任務完成
        camp: string
    longBo          +龍波
        actorCode: string
    print           控制台輸出
        type: string, text: string
    setGlobal       儲存全域變數
        key: string, type: string, value: int|string
    setObjectVar    儲存物件變數
        object: string, key: string, type: string, value: int|string
    wait            等待
        duration: int|string

Supported Checks:
    checkNumber     比較數字
        lhs: int|string, rhs: int|string, op: string
    checkString     比對字串
        value: string, matchKind: string, str: string 

Supported Triggers:
    actorDead       角色死亡
        actor: ActorMatch, varName: string, hitterVarName: string 
    actorFire       角色發動攻擊
        actor: ActorMatch, varName: string
    clickButton     告示牌按鈕
        actor: ActorMatch, varName: string, buttonId: string
    keyboardPressed 鍵盤按鍵
        actorId: string, varName: string, timing: string, key: string
    releasePower    發動技能
        actor: ActorMatch, varName: string, ability: string, weapon: string, preventDefault: bool, manaUsage: int|string
)";