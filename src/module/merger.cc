#include "syntheffect/module/merger.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    namespace module {
        Merger::Merger(double weight_right) {
            weight_ = weight_right;
        }

        void Merger::setWeight(double x, double min_in, double max_in) {
            weight_ = remapParam(x, 0, 1, min_in, max_in);
        }

        void Merger::update(const cv::Mat& left, const cv::Mat& right, cv::Mat& out) {
            cv::addWeighted(left, (1 - weight_), right, weight_, 0, out);
        }

        void Merger::fadeWeight(bool up) {
            weight_ = fadeParam(weight_, up, 0.05, 0, 1);
        }

        std::string Merger::stringify() {
            return str(boost::format("Merger: weight=%1%") % weight_);
        }
    };
};
