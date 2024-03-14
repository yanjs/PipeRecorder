#ifndef RECORDER_HPP
#define RECORDER_HPP

#include <memory>
#include <string>

class Recorder {
public:
  Recorder(const std::string &filename, size_t width, size_t height,
           size_t fps);
  Recorder(const Recorder &) = delete;
  Recorder &operator=(const Recorder &) = delete;
  void addFrame(const char *buffer);
  struct FileDeleter {
    inline void operator()(FILE *f) { fclose(f); }
  };
  using file_ptr = std::unique_ptr<FILE, FileDeleter>;
  enum class RecorderError {
    RECORDER_ERROR,
    SIGNAL_FAILED,
    POPEN_FAILED,
    PIPE_ERROR,
  };
  constexpr inline static const char *toString(RecorderError e) {
    switch (e) {
    case RecorderError::RECORDER_ERROR:
      return "Recorder Error";
    case RecorderError::SIGNAL_FAILED:
      return "signal() failed";
    case RecorderError::POPEN_FAILED:
      return "popen() failed";
    case RecorderError::PIPE_ERROR:
      return "Pipe is dead";
    }
    return "Unimplemented";
  }

private:
  std::string m_filename;
  size_t m_width;
  size_t m_height;
  size_t m_fps;
  file_ptr m_ffmpeg;
  pid_t m_child_pid;
};

#endif // RECORDER_HPP
