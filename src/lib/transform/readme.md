# Transform
### Legalization
##### Function Argument legalization
- Before: One `FunDefNode` can have positional argument or mismatched arugments.
- After: All positional argument will be converted into a named argument. If the named argument is mismatched or the number of arguments is incorrect, an error will be thrown.
##### Block legalization
- Before: One BlockBodyNode can have unlimited `TypedInstrSetNode`
- After: All `TypedInstrSetNode` will be removed. Exactly `Actions`, `Checks`, and `Triggers` will be transformed/created. Multiple `Actions`, `Checks`, or `Triggers` will be illegal.

### Propagation
##### Constant Value Propagation
##### Function Inlining

### Pipeline
1. Function Argument legalization
2. Block legalization
3. Function Inlining
4. Constant Value Propagation