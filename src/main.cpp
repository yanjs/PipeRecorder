#include "Recorder.hpp"
#include <cmath>
#include <cstring>
#include <iostream>

int main() try {
  constexpr size_t width = 800;
  constexpr size_t height = 600;
  constexpr size_t fps = 60;
  static uint8_t frame[height][width][3];
  Recorder recorder("output.mp4", width, height, fps);

  for (size_t t = 0; t < 360; t++) {
    int value = std::sin(t * std::numbers::pi / 180.f) * 100;
    frame[height / 2 + value][t][0] = 0;
    frame[height / 2 + value][t][1] = 128;
    frame[height / 2 + value][t][2] = 255;

    recorder.addFrame(reinterpret_cast<char *>(frame));
  }
  std::cout << "Exit normally" << std::endl;
  return 0;
} catch (Recorder::RecorderError &e) {
  std::cerr << "Something went wrong: " << Recorder::toString(e) << '\n';
} catch (...) {
  std::cerr << "Something not catched" << '\n';
}