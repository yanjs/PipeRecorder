#include "Recorder.hpp"
#include <csignal>
#include <sstream>

// Modified from public domain code. Thank you sjmulder
// https://github.com/sjmulder/ffmpeg-sample/blob/master/sample.c

Recorder::Recorder(const std::string &filename, size_t width, size_t height,
                   size_t fps)
    : m_filename(filename), m_width(width), m_height(height), m_fps(fps) {

  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    throw RecorderError::SIGNAL_FAILED;
  }
  // Launch ffmpeg
  std::stringstream ss_cmd;
  ss_cmd << "ffmpeg "
            "-loglevel warning "
            "-stats "
            "-f rawvideo "
            "-pixel_format rgb24 "
            "-video_size "
         << m_width << "x" << m_height
         << " "
            "-framerate "
         << fps
         << " "
            "-i - "
            "-pix_fmt yuv420p "
         << filename;
  auto s_cmd = ss_cmd.str();
  m_ffmpeg = decltype(m_ffmpeg)(popen(s_cmd.c_str(), "w"));
  if (!m_ffmpeg) {
    throw Recorder::RecorderError::POPEN_FAILED;
  }
#if 0
  std::stringstream ss_size;
  ss_size << m_width << "x" << m_height;
  auto s_size = ss_size.str();
  std::vector<char> v_size(s_size.begin(), s_size.end());
  v_size.push_back('\0');
  auto s_fps = std::to_string(m_fps);
  std::vector<char> v_fps(s_fps.begin(), s_fps.end());
  v_fps.push_back('\0');
  std::vector<char> v_filename(m_filename.begin(), m_filename.end());
  v_filename.push_back('\0');

  const char *argv[] = {
      "ffmpeg",     "-loglevel",       "warning", "-stats",      "-f",
      "rawvideo",   "-pixel_format",   "rgb24",   "-video_size", v_size.data(),
      "-framerate", v_fps.data(),      "-i",      "-",           "-pix_fmt",
      "yuv420p",    v_filename.data(), nullptr};

  int fds[2];


  if (pipe(fds) == -1)
    throw RecorderError::POPEN_FAILED;

  pid_t forked_pid = fork();
  m_child_pid = forked_pid;

  switch (forked_pid) {
  case -1:
    throw RecorderError::POPEN_FAILED;
  case 0:
    // in the child process
    // connect fds[0] to ffmpeg's stdin
    dup2(fds[0], STDIN_FILENO);
    // close the writing end fd before running ffmpeg
    close(fds[1]);
    // this cast is safe
    execvp("ffmpeg", const_cast<char *const *>(argv));
    // reached only when execvp failed
    throw Recorder::RecorderError::POPEN_FAILED;
  default:
    break;
  }

  // in the parent process
  // close the reading end fd
  close(fds[0]);
  m_ffmpeg = decltype(m_ffmpeg)(fdopen(fds[1], "w"));
  // fds[1] is closed in fclose(m_ffmpeg)
  return;
#endif
}

void Recorder::addFrame(const char *buffer) {
  auto retn = std::fwrite(buffer, m_width * m_height * 3, 1, m_ffmpeg.get());
  if (retn != 1) {
    throw RecorderError(RecorderError::PIPE_ERROR);
  }
}