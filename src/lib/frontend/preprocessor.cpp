#include "frontend/preprocessor.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

struct PreprocessedInputFile {
  std::string filename;
  std::string content;
};

std::string getParentPath(const std::string &path) {
  size_t lastSlash = path.find_last_of("/\\");
  if (lastSlash == std::string::npos) {
    return "";
  }
  return path.substr(0, lastSlash + 1);
}

bool isAbsolutePath(const std::string &path) {
  if (path.empty())
    return false;
  if (path[0] == '/')
    return true;
  return path.size() > 2 && std::isalpha(static_cast<unsigned char>(path[0])) &&
         path[1] == ':' && (path[2] == '/' || path[2] == '\\');
}

bool processFile(const std::string &currentFilename,
                 const std::string &currentContent,
                 std::unordered_set<std::string> &visited,
                 std::vector<std::pair<std::string, std::string>> &output) {
  if (!visited.insert(currentFilename).second) {
    return true;
  }

  const std::string includePrefix = "#include \"";
  std::istringstream contentStream(currentContent);
  std::ostringstream processedContentStream;
  std::string line;
  std::size_t lineNumber = 0;
  while (std::getline(contentStream, line)) {
    ++lineNumber;
    const std::string location =
        currentFilename + ":" + std::to_string(lineNumber);
    const std::size_t includeLoc = line.find(includePrefix);

    if (includeLoc == std::string::npos) {
      processedContentStream << line << "\n";
      continue;
    }

    if (includeLoc != 0)
      return std::cerr
                 << "SyntaxError: '#include \"' must be at the beginning of "
                    "the line in "
                 << location << "\n",
             false;

    const std::size_t pathStart = includePrefix.size();
    const std::size_t pathEnd = line.find('"', pathStart);
    if (pathEnd == std::string::npos)
      return std::cerr << "SyntaxError: missing closing '\"' in include "
                          "directive in "
                       << location << "\n",
             false;

    const std::size_t trailingNonSpace =
        line.find_first_not_of(" \t", pathEnd + 1);
    if (trailingNonSpace != std::string::npos)
      return std::cerr << "SyntaxError: unexpected token after include path in "
                       << location << "\n",
             false;

    std::string includePath = line.substr(pathStart, pathEnd - pathStart);
    if (includePath.size() < 5 ||
        includePath.substr(includePath.size() - 5) != ".twge")
      return std::cerr << "SyntaxError: include path must end with '.twge' in "
                       << location << "\n",
             false;

    if (!isAbsolutePath(includePath))
      includePath = getParentPath(currentFilename) + includePath;

    std::ifstream includeFile(includePath);
    if (!includeFile.is_open())
      return std::cerr << "PreprocessError: cannot open included file '"
                       << includePath << "' in " << location
                       << ". Check that the file exists and the include path "
                          "is correct.\n",
             false;

    std::string includeContent((std::istreambuf_iterator<char>(includeFile)),
                               std::istreambuf_iterator<char>());
    processedContentStream << "\n";
    if (!processFile(includePath, includeContent, visited, output))
      return false;
  }

  output.push_back({currentFilename, processedContentStream.str()});
  return true;
}

bool Preprocessor::preprocess(
    std::vector<std::pair<std::string, std::string>> &sourceFiles) {
  std::unordered_set<std::string> visited;
  sourceFiles.clear();
  return processFile(filename, content, visited, sourceFiles);
}