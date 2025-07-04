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

```c
// Basic Node
ModuleNode := [BlockNode | MetadataNode]*
- BlockNode := BlockToken IdentifierToken OpenCurToken [ MetadataNode | ActionsNode | ChecksNode | TriggersNode] CloseCurToken
  - ActionsNode := ActionsToken OpenCurToken ActionNode* CloseCurToken
    - ActionNode := IdentifierToken [DotToken IdentifierToken]* OpenParToken PositionalArgNode? CloseParToken SemicolonToken
  - ChecksNode := ChecksToken OpenCurToken CheckNode* CloseCurToken
  - TriggersNode := TriggersToken OpenCurToken TriggerNode* CloseCurToken
- MetadataNode := MetadataToken AssignToken ValueNode SemicolonToken
// Arguments
PositionalArgNode = [IdentifierToken AssignToken ValueNode [CommaToken NamedArgNode]?] | 
                    [ValueNode [CommaToken PositionalArgNode]?]
NamedArgNode = IdentifierToken AssignToken ValueNode [CommaToken NamedArgNode]?
// Value
ValueNode := StringValueNode | IntValueNode | BoolValueNode
StringValueNode := StringToken
IntValueNode := IntToken
BoolValueNode := TrueToken | FalseToken
```
