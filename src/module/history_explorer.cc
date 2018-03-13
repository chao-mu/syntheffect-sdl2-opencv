#include "syntheffect/module/history_explorer.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    namespace module {
        HistoryExplorer::HistoryExplorer() {
            cv::Mat saved_frame_;
            cv::Mat last_frame_;
            active_ = false;
            history_weight_ = 0.5;
        }

        void HistoryExplorer::setHistoryWeight(double param) {
            history_weight_ = param;
        }

        void HistoryExplorer::update(const cv::Mat& in, cv::Mat& out) {
            in.copyTo(last_frame_);

            if (active_) {
                cv::addWeighted(in, (1 - history_weight_), saved_frame_, history_weight_, 0, out);
            } else {
                in.copyTo(out);
            }
        }

        void HistoryExplorer::fadeHistoryWeight(bool up) {
            history_weight_ = fadeParam(history_weight_, up, 0.05);
        }

        void HistoryExplorer::start() {
            if (!active_) { 
                active_ = true;
                last_frame_.copyTo(saved_frame_);
            }
        }

        void HistoryExplorer::stop() {
            active_ = false;
        }

        std::string HistoryExplorer::stringify() {
            return str(boost::format("HistoryExplorer: history_weight=%1%") % history_weight_);
        }
    };
};
