#include "syntheffect/module/module.h"

#include <SDL.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <assert.h>
#include <boost/format.hpp>

namespace syntheffect {
    namespace module {
        double Module::remapParam(double param, double out_min, double out_max, double in_min, double in_max) {
            return (param - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
        }

        int Module::remapParamInt(double param, double out_min, double out_max, double in_min, double in_max) {
            return int(round(remapParam(param, out_min, out_max, in_min, in_max)));
        }

        int Module::remapParamOdd(double param, double out_min, double out_max, double in_min, double in_max) {
            int x = remapParamInt(param, out_min, out_max, in_min, in_max);
            assert(out_max - out_min > 0);

            if (x % 2 == 0 && x != 0) {
                int shiftedDown = x - 1;
                int shiftedUp = x + 1;

                if (shiftedDown >= out_min) {
                    x = shiftedDown;
                } else if (shiftedUp <= out_max) {
                    x = shiftedUp;
                } else {
                    assert(false);
                }
            }


            return x;
        }

        double Module::fadeParam(double current, bool up, double step, double out_min, double out_max) {
            if (up) {
                current += step;
                if (current > out_max) {
                    current = out_max;
                }
            } else {
                current -= step;
                if (current < out_min) {
                    current = out_min;
                }
            }

            return current;
        }

        void Module::start() {
            active_ = true;
        }

        void Module::stop() {
            active_ = false;
        }

        void Module::toggle() {
            if (active_) {
                stop();
            } else {
                start();
            }
        }
    };
};
