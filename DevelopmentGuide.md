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
directory structure:
```
test
├── fail
│   ├── codegen
│   ├── lexer
│   └── transform
└── pass
    ├── codegen
    └── transform
```
In the leaf directory, there must be
- exactly one `test.twge` file for the `twgec` compilation.
- If that is under the `fail` directory, `error.log` is expected to be there.
- If that is under the `pass/codegen` directory, `output.events` is expected to be there.
- If that is under the `pass/transform` directory, `print.twge` is expected to be there.

## Future Plan
- Support Chinese Instruction
- Support guideline markdown generation
- Fix incorrect Chinese position calculation
- Support `$var` syntax for runtime variable
- Support `let` syntax for mutable local variable