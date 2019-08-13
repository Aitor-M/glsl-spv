#include "shader_list.h"

#include <windows.h>

void glspv::ShaderList::add(const ShaderData& data) {
  list_.push_back(data);
}

void glspv::ShaderList::removeAt(size_t index) {
  auto listi = list_.begin();
  std::advance(listi, index);
  list_.erase(listi);
}

void glspv::ShaderList::draw() {
  uint32_t index = 0;
  for (auto itr = list_.begin(); itr != list_.end(); ) {
    itr->draw(index);
    if (itr->shouldBeRemoved()) {
      itr = list_.erase(itr);
    } else {
      itr++; 
      index++;
    }
  }
}

void glspv::ShaderList::clear() {
  list_.clear();
}

static void CompileScript(const std::string& vulkanSdk, const std::string& input, const std::string& ouput) {
  // additional information
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  // set the size of the structures
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));

  std::string command = vulkanSdk + " " + input + " -o " + ouput;

  // start the program up
  CreateProcess(NULL,   // the path
    const_cast<LPSTR>(command.c_str()),        // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory 
    &si,            // Pointer to STARTUPINFO structure
    &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
  );
  // Close process and thread handles. 
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

void glspv::ShaderList::compile(const std::string& vulkanSdk) {
  for (ShaderData& data : list_) {
    CompileScript(vulkanSdk, data.inputFilePath_, data.outputPath_ + "\\" + data.outputName_);
  }
}
