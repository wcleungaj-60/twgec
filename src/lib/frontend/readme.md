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

##### Overview

![AST image](/assets/frontend/ast.png)

##### Regular Expression
```c
// Global Scope
ModuleNode := [ MetadataNode | BlockNode | ConstDefNode | FunDefNode]*
- MetadataNode := MetadataToken AssignToken ExpNode
- BlockNode := BlockToken IdentifierToken [BlockBodyNode | [AssignToken InstructionNode]]
- ConstDefNode := ConstToken IdentifierToken AssignToken ExpNode
- FunDefNode := DefToken IdentifierToken ParamDefsNode ColonToken [TypedInstrSetNode | [BlockToken BlockBodyNode]]
  - ParamDefsNode = OpenParToken [IdentifierToken [CommaToken IdentifierToken]*]? CloseParToken
  - BlockBodyNode = OpenCurToken [ MetadataNode | TypedInstrSetNode ]* CloseCurToken
// Instruction Set Scope
TypedInstrSetNode = [ActionsDefNode | ChecksDefNode | TriggersDefNode] InstrSetNode
- InstrSetNode := OpenCurToken CompositeInstrNode* CloseCurToken
  - CompositeInstrNode := BranchNode | ForLoopNode | InstructionNode
    - BranchNode := IfToken OpenParToken ExpNode CloseParToken InstrSetNode [ElseIfToken InstrSetNode]* [ElseToken InstrSetNode]?
    - ForLoopNode := ForToken IdentifierToken InToken ExpNode EllipsisToken ExpNode InstrSetNode
// Instruction Scope
InstructionNode := IdentifierToken [DotToken IdentifierToken]* ParamAppsNode
- ParamAppsNode = OpenParToken PositionalParamAppsNode? CloseParToken
  - PositionalParamAppsNode = [IdentifierToken AssignToken ExpNode [CommaToken NamedParamAppsNode]?] | 
                  [ExpNode [CommaToken PositionalParamAppsNode]?]
  - NamedParamAppsNode = IdentifierToken AssignToken ExpNode [CommaToken NamedParamAppsNode]?
// Expression
ExpNode = LogicalOrExpNode
- LogicalOrExpNode = LogicalAndExpNode [ OrToken LogicalAndExpNode ]*
- LogicalAndExpNode = EqualityExpNode [ AndToken EqualityExpNode ]*
- EqualityExpNode = RelationalExpNode [ [ EqualToken | NotEqualToken] RelationalExpNode ]*
- RelationalExpNode = AdditiveExpNode [ [LessThanToken | GreaterThanToken | LessThanEqualToken | GreaterThanEqualToken] AdditiveExpNode ]*
- AdditiveExpNode = MultiplicativeExpNode [ [AddToken | SubToken] MultiplicativeExpNode ]*
- MultiplicativeExpNode = PrimaryExpNode [ [MulToken | DivToken | ModToken] PrimaryExpNode ]*
- PrimaryExpNode = ValueNode | [OpenParToken ExpNode CloseParToken]
ValueNode := StringValueNode | IntValueNode | BoolValueNode | VariableValueNode
- StringValueNode := StringToken
- IntValueNode := IntToken
- BoolValueNode := TrueToken | FalseToken
- VariableValueNode := IdentifierToken
- PointValueNode := PointToken OpenParToken IntToken CommaToken IntToken CloseParToken
- ListValueNode := OpenSqrToken [ExpNode [CommaToken ExpNode]*]? CloseSqrToken
```
