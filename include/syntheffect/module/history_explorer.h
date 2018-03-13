#ifndef SYNTHEFFECT_MODULE_HISTORY_EXPLORER_MODULE_H
#define SYNTHEFFECT_MODULE_HISTORY_EXPLORER_MODULE_H

#include <opencv2/core/mat.hpp>

#include "syntheffect/module/module.h"

namespace syntheffect {
    namespace module {
        class HistoryExplorer : Module {
            public:
                HistoryExplorer();
                void update(const cv::Mat& in, cv::Mat& out);
                void start();
                void stop();
                void setHistoryWeight(double param);
                void fadeHistoryWeight(bool up);
                std::string stringify();

            private:
                cv::Mat saved_frame_;
                cv::Mat last_frame_;
                bool active_;
                double history_weight_;
        };
    };
};
#endif
