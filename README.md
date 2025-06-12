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
The `transform` directory contains the IR transformation pass, such as alias inlining. It converts the AST into a legal form for code generation.
<br>
The `codegen` directory will generate the `events.json` from a legal AST.

## test
Unit tests and the test script will be written under this directory.

In the leaf directory, there must be
- exactly one `test.twge` file for the `twgec` compilation.
- either an `output.events` or an `error.log` file for the compilation result verification.

# Language Feature

## Alias
Instead of repeating the actor parameters as follows:
```
block my_block {
    actions {
        addActor(camp = "royal", x = 5, y = 10, name = "狙擊手", hp = "150", range = "0");
        addActor(camp = "royal", x = 10, y = 5, name = "狙擊手", hp = "150", range = "0");
        addActor(camp = "royal", x = 8, y = 18, name = "狙擊手", hp = "150", range = "0");
        addActor(camp = "royal", x = 18, y = 8, name = "狙擊手", hp = "150", range = "0");
    }
}
```
The `alias` keyword is supported. It acts like a function that allows the user to customize the alias parameters to convert that `alias` into a valid action as follows:
```
alias addSnipperActor(camp, x, y) {
    addActor(camp = camp, x = x, y = y, name = "狙擊手", hp = "150", range = "0");
}

block my_block {
    actions {
        addSnipperActor(camp = "royal", x = 5, y = 10);
        addSnipperActor(camp = "royal", x = 10, y = 5);
        addSnipperActor(camp = "royal", x = 8, y = 18);
        addSnipperActor(camp = "royal", x = 18, y = 8);
    }
}
```