#include "syntheffect/module/writer.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    namespace module {
        Writer::Writer(const std::string filename, int width, int height, double fps) {
            cv::Size size = cv::Size(width, height);
            assert(writer_.open(filename, CV_FOURCC('M', 'J', 'P', 'G'), fps, size));
        }

        void Writer::release() {
            writer_.release();
        }

        void Writer::update(const cv::Mat& in, cv::Mat& out) {
            in.copyTo(out);

            if (!active_) {
                return;
            }

            writer_.write(out);
        }
    };
};
