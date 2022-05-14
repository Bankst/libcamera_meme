#include "camera_runner.h"

CameraRunner::CameraRunner(int width, int height, const std::string& id)
	: CameraRunner(width, height, CameraManagerUtils::GetCameraById(id)) {}

CameraRunner::CameraRunner(int width, int height, std::shared_ptr<libcamera::Camera> camera)
 : m_width(width), m_height(height), m_grabber(camera, width, height) {
	//   = CameraGrabber(m_cameraPtr, m_width, m_height);

	auto allocer = DmaBufAlloc("/dev/dma_heap/linux,cma");
	m_stride = m_grabber.streamConfiguration().stride;

	m_grabber.setOnData([&](libcamera::Request *request) {
		m_cameraQueue.push(request);
	});

	for (int i = 0; i < 3; i++) {
		m_fds.push_back(allocer.alloc_buf(width * height * 4));
	}
}

void CameraRunner::Start() {
	m_thresholdThread = std::thread([&, this]() {
		m_thresholder.start(m_fds);
		auto colorspace = m_grabber.streamConfiguration().colorSpace.value();

		m_thresholder.setOnComplete([&](int fd) {
			m_gpuQueue.push(fd);
		});

		// double gpuTimeAvgMs = 0;

		while (threshold_thread_run) {
			auto request = m_cameraQueue.pop();

			if (!request) {
				break;
			}

			// in Nanoseconds, from the Linux CLOCK_BOOTTIME
			// auto sensorTimestamp = request->controls().get(libcamera::controls::SensorTimestamp);
			// std::cout << "Got cam ts: " << sensorTimestamp << std::endl;

			auto planes = request->buffers().at(m_grabber.streamConfiguration().stream())->planes();

			std::array<GlHsvThresholder::DmaBufPlaneData, 3> yuv_data {{
				{planes[0].fd.get(),
				static_cast<EGLint>(planes[0].offset),
				static_cast<EGLint>(m_stride)},
				{planes[1].fd.get(),
				static_cast<EGLint>(planes[1].offset),
				static_cast<EGLint>(m_stride / 2)},
				{planes[2].fd.get(),
				static_cast<EGLint>(planes[2].offset),
				static_cast<EGLint>(m_stride / 2)},
				}};

			// auto begintime = steady_clock::now();
			m_thresholder.testFrame(yuv_data, encodingFromColorspace(colorspace), rangeFromColorspace(colorspace));

			// std::chrono::duration<double, std::milli> elapsedMillis = steady_clock::now() - begintime;
			// if (elapsedMillis > 0.9ms) {
				// gpuTimeAvgMs = approxRollingAverage(gpuTimeAvgMs, elapsedMillis.count());
				// std::cout << "GLProcess: " << gpuTimeAvgMs << std::endl;
			// }
			m_grabber.requeueRequest(request);
		}
	});

	m_displayThread = std::thread([&, this]() {
		std::unordered_map<int, unsigned char *> mmaped;

		for (auto fd: m_fds) {
			auto mmap_ptr = mmap(nullptr, m_width * m_height * 4, PROT_READ, MAP_SHARED, fd, 0);
			if (mmap_ptr == MAP_FAILED) {
				throw std::runtime_error("failed to mmap pointer");
			}
			mmaped.emplace(fd, static_cast<unsigned char *>(mmap_ptr));
		}

		cv::Mat threshold_mat(m_height, m_width, CV_8UC1);
		unsigned char *threshold_out_buf = threshold_mat.data;
		cv::Mat color_mat(m_height, m_width, CV_8UC3);
		unsigned char *color_out_buf = color_mat.data;

		// double copyTimeAvgMs = 0;

		while (display_thread_run) {
			auto fd = m_gpuQueue.pop();
			if (fd == -1) {
				break;
			}

			// auto begintime = steady_clock::now();
			auto input_ptr = mmaped.at(fd);
			int bound = m_width * m_height;

			for (int i = 0; i < bound; i++) {
				std::memcpy(color_out_buf + i * 3, input_ptr + i * 4, 3);
				threshold_out_buf[i] = input_ptr[i * 4 + 3];
			}

			// pls don't optimize these writes out compiler
			std::cout << reinterpret_cast<uint64_t>(threshold_out_buf) << " " << reinterpret_cast<uint64_t>(color_out_buf) << std::endl;

			m_thresholder.returnBuffer(fd);

			// std::chrono::duration<double, std::milli> elapsedMillis = steady_clock::now() - begintime;
			// if (elapsedMillis > 0.9ms) {
				// copyTimeAvgMs = approxRollingAverage(copyTimeAvgMs, elapsedMillis.count());
				// std::cout << "Copy: " << copyTimeAvgMs << std::endl;
			// }

			// cv::imshow("cam_color", color_mat);
			// cv::imshow("cam_single", threshold_mat);
			// cv::waitKey(1);
		}
	});

	std::this_thread::sleep_for(std::chrono::seconds(2));

	m_grabber.startAndQueue();
}

CameraRunner::~CameraRunner() {
	Stop();
}

void CameraRunner::Stop() {
  threshold_thread_run = false;
  m_thresholdThread.join();
	display_thread_run = false;
	m_displayThread.join();
}

std::shared_ptr<libcamera::Camera> CameraManagerUtils::GetCameraById(const std::string &id) {
	auto cameras = instance->cameras();
	for (auto cam : cameras) {
		if (cam->id() == id) return cam;
	}

	// return nullptr if none found
	return std::shared_ptr<libcamera::Camera>();
};