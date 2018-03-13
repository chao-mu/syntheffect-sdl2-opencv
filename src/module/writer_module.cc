#include "syntheffect/module/writer_module.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    WriterModule::WriterModule(const std::string filename, int width, int height, double fps) {
        cv::Size size = cv::Size(width, height);
        assert(writer_.open(filename, CV_FOURCC('M', 'J', 'P', 'G'), fps, size));
    }

    void WriterModule::release() {
        writer_.release();
    }

    void WriterModule::update(const cv::Mat& in, cv::Mat& out) {
        in.copyTo(out);

        if (!active_) {
            return;
        }

        writer_.write(out);
    }
};
