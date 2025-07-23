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
ModuleNode := [BlockNode | MetadataNode | AliasNode]*
- BlockNode := BlockToken IdentifierToken OpenCurToken [ MetadataNode | ActionsNode | ChecksNode | TriggersNode] CloseCurToken
  - ActionsNode := ActionsToken OpenCurToken InstructionNode* CloseCurToken
  - ChecksNode := ChecksToken OpenCurToken CheckNode* CloseCurToken
  - TriggersNode := TriggersToken OpenCurToken TriggerNode* CloseCurToken
    - InstructionNode := IdentifierToken [DotToken IdentifierToken]* OpenParToken ArgsNode CloseParToken SemicolonToken
- MetadataNode := MetadataToken AssignToken ExpNode SemicolonToken
- AliasNode := AliasToken IdentifierToken OpenParToken ParamsNode CloseParToken OpenCurToken InstructionNode* CloseCurToken
// Parameter (The function definition)
ParamsNode = [IdentifierToken [CommaToken IdentifierToken]*]?
// Arguments (The function application)
ArgsNode = PositionalArgsNode?
PositionalArgsNode = [IdentifierToken AssignToken ExpNode [CommaToken NamedArgsNode]?] | 
                    [ExpNode [CommaToken PositionalArgsNode]?]
NamedArgsNode = IdentifierToken AssignToken ExpNode [CommaToken NamedArgsNode]?
// Operation
OperationNode = PlusToken
// Expression
ExpNode = ValueNode [OperationNode ExpNode]?
// Value
ValueNode := StringValueNode | IntValueNode | BoolValueNode | VariableValueNode
StringValueNode := StringToken
IntValueNode := IntToken
BoolValueNode := TrueToken | FalseToken
VariableValueNode := IdentifierToken
```
