# Architecture

`codegen.cpp` is the main interface of the code generator.
`transformer.cpp` provides several utils which can enhance the transformation from ast to codegen.

`metadata.cpp` handles the metadata data such as block's delay or game config.
`action.cpp` handles the action code generation.
`check.cpp` handles the check code generation.
`trigger.cpp` handles the trigger code generation.