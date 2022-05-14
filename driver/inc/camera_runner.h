#include <libcamera/camera.h>
#include <libcamera/camera_manager.h>
#include <libcamera/control_ids.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <cstring>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <sys/mman.h>

#include "dma_buf_alloc.h"
#include "gl_hsv_thresholder.h"
#include "camera_grabber.h"
#include "libcamera_opengl_utility.h"
#include "concurrent_blocking_queue.h"

class CameraRunner {
public:
  CameraRunner(int width, int height, const std::string &id);
  CameraRunner(int width, int height, std::shared_ptr<libcamera::Camera> camera);
  ~CameraRunner();

  void Start();
  void Stop();

private:
  const int m_width;
  const int m_height;

  std::thread m_thresholdThread;
  std::thread m_displayThread;
  // std::shared_ptr<libcamera::Camera> m_cameraPtr;

  bool threshold_thread_run = true;
  bool display_thread_run = true;

  std::vector<int> m_fds;
  CameraGrabber m_grabber;
  ConcurrentBlockingQueue<libcamera::Request *> m_cameraQueue;
  ConcurrentBlockingQueue<int> m_gpuQueue;
  GlHsvThresholder m_thresholder = GlHsvThresholder(m_width, m_height);
  unsigned int m_stride = 0;
};

class CameraManagerUtils {
public:
  inline static std::unique_ptr<libcamera::CameraManager> instance = std::make_unique<libcamera::CameraManager>();
  static std::shared_ptr<libcamera::Camera> GetCameraById (const std::string &id);
private:
  CameraManagerUtils() {};
};
