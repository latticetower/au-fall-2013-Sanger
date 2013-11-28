#include <fstream>
#include <string>

extern "C" __declspec(dllexport) int read_from_file(std::string filename);

int main() {
  return read_from_file("input.txt") ;
}
int read_from_file(std::string filename) {
  int x;
  std::ifstream f(filename);
  f >> x;
  f.close();
  return x;
}