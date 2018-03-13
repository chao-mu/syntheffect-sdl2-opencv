#ifndef SYNTHEFFECT_MODULE_WRITER_MODULE
#define SYNTHEFFECT_MODULE_WRITER_MODULE

#include <opencv2/videoio.hpp>

#include "syntheffect/module.h"

namespace syntheffect {
    class WriterModule : public Module {
        public:
           WriterModule(const std::string filename, int width, int height, double fps);
           void update(const cv::Mat& in, cv::Mat& out);
           void release();

        private:
           cv::VideoWriter writer_;
    };
}
#endif
