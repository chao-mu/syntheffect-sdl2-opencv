#include "syntheffect/module/color_tweak.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    namespace module {
        ColorTweak::ColorTweak() {
        }

        void ColorTweak::update(const cv::Mat& in, cv::Mat& out) {
            if (!active_) {
                in.copyTo(out);
                return;
            }
            //cv::Size blur_size(5, 5);
            //cv::blur(in, out, blur_size);

            cv::cvtColor(in, out, cv::COLOR_BGR2HSV);

            // The rolling offset to use to tinker with our pixels.
            uint32_t offset = SDL_GetTicks() / 10;
           
            // Iterate over each pixel
            cv::MatIterator_<cv::Vec3b> it = out.begin<cv::Vec3b>();
            cv::MatIterator_<cv::Vec3b> it_end = out.end<cv::Vec3b>();
            for(; it != it_end; ++it) {
                cv::Vec3b& pixel = *it; // reference to pixel in image
                pixel[0] = (pixel[0] + offset) % 256;
                pixel[2] = (pixel[2] + (offset/2)) % 256;
            }

            cv::cvtColor(out, out, cv::COLOR_HSV2BGR);
        }
    };
};
