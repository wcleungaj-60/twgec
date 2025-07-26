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
// Global Scope
ModuleNode := [ MetadataNode | BlockNode | ConstDefNode | FunDefNode]*
- MetadataNode := MetadataToken AssignToken ExpNode
- BlockNode := BlockToken IdentifierToken [BlockBodyNode | InstructionNode]
- ConstDefNode := ConstToken IdentifierToken AssignToken ExpNode
- FunDefNode := DefToken IdentifierToken ParamDefsNode ColonToken [TypedInstrSetDefNode | [BlockToken BlockBodyNode]]
  - ParamDefsNode = OpenParToken [IdentifierToken [CommaToken IdentifierToken]*]? CloseParToken
// Block Scope
BlockBodyNode = OpenCurToken [ MetadataNode | TypedInstrSetNode ]* CloseCurToken
- TypedInstrSetDefNode = ActionsDefNode | ChecksDefNode | TriggersDefNode
  - ActionsDefNode := ActionsToken InstrSetNode
  - ChecksDefNode := ChecksToken InstrSetNode
  - TriggersDefNode := TriggersToken InstrSetNode
// Instruction Set Scope
InstrSetNode := OpenCurToken [BranchNode | InstructionNode]* CloseCurToken
BranchNode := IfToken InstrSetNode [ElseIfToken InstrSetNode]* [ElseToken InstrSetNode]?
// Instruction Scope
InstructionNode := IdentifierToken [DotToken IdentifierToken]* ParamAppsNode
- ParamAppsNode = OpenParToken PositionalParamAppsNode? CloseParToken
  - PositionalParamAppsNode = [IdentifierToken AssignToken ExpNode [CommaToken NamedParamAppsNode]?] | 
                  [ExpNode [CommaToken PositionalParamAppsNode]?]
  - NamedParamAppsNode = IdentifierToken AssignToken ExpNode [CommaToken NamedParamAppsNode]?
// Expression
ExpNode = ValueNode [OperationNode ExpNode]?
- OperationNode = PlusToken
- ValueNode := StringValueNode | IntValueNode | BoolValueNode | VariableValueNode
  - StringValueNode := StringToken
  - IntValueNode := IntToken
  - BoolValueNode := TrueToken | FalseToken
  - VariableValueNode := IdentifierToken
  - PointValueNode := OpenParToken IntToken CommaToken IntToken CloseParToken
  - ListValueNode := OpenSqrToken [ExpNode [CommaToken ExpNode]*]? CloseSqrToken
```
