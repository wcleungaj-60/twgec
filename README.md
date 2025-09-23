# twgec
`twgec` stands for the `twilight game.events compiler`. It compiles the `.twge` DSL into the `game.events` that can be used in the [Twilight War Game Creation](https://code.gamelet.com/).

# Setup the twgec

## Setup Syntax Highlighter

To setup the highlighter, please make sure you have already installed the `vsce` package. If not, you can run
```
npm install -g vsce
```
After, you have to go to the highlighter repo and build the vscode extension as below:
```
cd tool/twge-syntax-highlighter
vsce package
```
The vscode extension (`twge-syntax-highlighter-<version>.vsix`) will be built. You just need to right click that extension and choose `Install Extension VSIX`.

With the syntax highlighter. The `.twge` will have the syntax highlighting as below:

![The highlighted .twge syntax](/assets/readme/syntax-highlighting.png)

## Build twgec

To build the twgec (twge compiler), you have to execute the `build.sh` as below:
```
./bash.sh
```
After, the build directory will be created. To run the `twgec` with a simple testcase. You might run
```
build/twgec test/codegen/actions/success/console/test.twge
```
to see can the `game.events` be generated (`ls game.events`)

# Start to code

## Help

By running the following command:
```
build/twgec --help
```
the guideline of how to use the `twgec` will be shown.

## Simple Example

![A simple example of twge](/assets/readme/simple-twge.png)

As the code snippet above, `block` is the basic unit of the game event.

Under a `block`, you can add the `triggers`, `checks`, and `actions` here these have the exactly same meaning as the what [Twilight War Game Creation](https://code.gamelet.com/) have.

Under a `block`, some metadata such as `__repeat__` and `__delay__` etc. can be set.

With this simple example, the `game.events` can be generated.

## Advanced Feature

### Function Definition

#### Block function

By referring to the `Simple Example` Section, the implemenation of the block `chooseJobS1` and `chooseJobS2` are highly repeated. In order to increase the usibility, function definition should be used.

![The twge example with block function definition](/assets/readme/fun-def-block.png)

As the code snippet above, `def <functionDefinitionName>(<param>...): block {}` can be used here.  `def` means that is a function definition. `: block` means that is the function definition of the class `block`. The `block`\'s implementation can be completed inside the bracket (`{}`).

In order to call the function, the syntax should be `block <blockName> = <functionDefintionName>(<arg>...);` as the code snippet above.

#### Actions function

![The twge example with action function definition](/assets/readme/fun-def-action.png)

Sometimes, the `actions`, `triggers`, and `checks` might have the duplicated logic. In order to reuse the implementation, function can be used as above (see `setInt`). It is similiar to the block function definition. But remember, the type of the definition should be `: actions`, `: triggers`, or `: checks` instead of `: block`.