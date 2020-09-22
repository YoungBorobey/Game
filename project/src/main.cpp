#include <no_gui.h>
#include <string.h>
int main(int argc, const char *argv[]) {
  const char *filepath = nullptr;
  if (argc > 1) {
    for (int i = 0; i < argc; i++) {
      if (!strcmp(argv[i], "--map")) {
        filepath = argv[i + 1];
        return no_gui(filepath);
      }
    }
  } else {
    filepath = "res/map.map";
    return no_gui(filepath);
  }
  return 0;
}
