# twgec
The transpiler for twilight game.events

# Folder Architecture

## example

Write a `twge` example under this directory. Every time you build the `twgec` or run the test script, all the `twge` files under this directory will be tested with a smoke test. The code generation result will not be checked. It only ensures that none of the examples have compilation errors.

## src

The `include` directory contains the header files.
<br>
The `lib` directory contains the cpp files.
<hr>

The `frontend` directory contains the lexer and parser. It converts the `twge` file into an AST.
<br>
The `transform` directory contains the IR transformation pass, such as function inlining. It converts the AST into a legal form for code generation.
<br>
The `codegen` directory will generate the `events.json` from a legal AST.

## test
Unit tests and the test script will be written under this directory.

In the leaf directory, there must be
- exactly one `test.twge` file for the `twgec` compilation.
- either an `output.events` or an `error.log` file for the compilation result verification.

# Language Feature

## Trait
```
// Trait definition
@default_weapon = addActor(weapon1_default = true, weapon2_default = true);

// Trait Example
block myBlock {
  actions {
    addActor(name = "actor1", weapon1 = "knife") @default_weapon;
    addActor(name = "actor2", weapon1 = "knife") @default_weapon;
  }
}
```

## Function
```
// Function Definition
def addSnipperActor(camp, x, y) {
    addActor(camp = camp, x = x, y = y, name = "狙擊手", hp = "150", range = "0");
}

// Function Example
block my_block {
    actions {
        addSnipperActor(camp = "royal", x = 5, y = 10);
        addSnipperActor(camp = "royal", x = 10, y = 5);
    }
}
```

## File including
```
#include "../occupation/ninja.twge"
#include "../occupation/mage.twge"
#include "../occupation/warrior.twge"
```

|Langauge Feature|Abstraction Level|Description|
| --- | --- | --- |
|Trait|One instruction multiple param| - Overloading is supported when trait is used by different instruction <br> - Overwritting is not supported (Earlier trait is kept)|
|Function|One block multiple instructions| - Primitive parameter can be passed <br> - Trait cannot be passed|
|File including|One file multiple block| - Block redefinition will be checked|