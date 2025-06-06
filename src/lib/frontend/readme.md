# Parser
### Symbol and Naming
**Logical Symbol**

`|` means or

`?` means optional

`*` means zero to many

`[]` defines the parsing priority

**Named Symbol**

`<xxx>Node` is the AST node that is declared in AST.h

`<xxx>Token` is the token that is declared in Token.h

`<XXX>` with all uppercase characters is an intermediate expression

### DSL Parsing Logic

**AST node hierarchy**

```c
ModuleNode := [BlockNode | MetadataNode]*
- MetadataNode := MetadataToken AssignToken StringToken SemicolonToken
- BlockNode := BlockToken IdentifierToken OpenCurToken [ActionsNode | ChecksNode | TriggersNode] CloseCurToken
  - ActionsNode := ActionsToken OpenCurToken ActionNode* CloseCurToken
    - ActionNode := IdentifierToken [DotToken IdentifierToken]* OpenParToken ARGS? CloseParToken SemicolonToken
  - ChecksNode := ChecksToken OpenCurToken CheckNode* CloseCurToken
  - TriggersNode := TriggersToken OpenCurToken TriggerNode* CloseCurToken
```

**Intermediate expression**

```c
ARGS = [IdentifierToken AssignToken StringToken [CommaToken NAMED_ARGS]?] | 
       [STRING [CommaToken ARGS]?]
NAMED_ARGS = IdentifierToken AssignToken StringToken [CommaToken NAMED_ARGS]?
```