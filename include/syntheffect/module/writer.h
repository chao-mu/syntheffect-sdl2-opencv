#ifndef SYNTHEFFECT_MODULE_WRITER_H
#define SYNTHEFFECT_MODULE_WRITER_H

#include <opencv2/videoio.hpp>

#include "syntheffect/module/module.h"

namespace syntheffect {
    namespace module {
        class Writer : public Module {
            public:
                Writer(const std::string filename, int width, int height, double fps);
                void update(const cv::Mat& in, cv::Mat& out);
                void release();

            private:
                cv::VideoWriter writer_;
        };
    };
};
#endif
