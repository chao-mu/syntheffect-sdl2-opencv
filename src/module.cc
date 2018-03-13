#include "syntheffect/module.h"

#include <SDL.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <assert.h>
#include <boost/format.hpp>

#define MIN_PARAM 0
#define MAX_PARAM 1

namespace syntheffect {
    double Module::remapParam(double param, double out_min, double out_max) {
        double in_min = MIN_PARAM;
        double in_max = MAX_PARAM; 

        return (param - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    int Module::remapParamInt(double param, double out_min, double out_max) {
        return int(round(remapParam(param, out_min, out_max)));
    }

    int Module::remapParamOdd(double param, double min, double max) {
        int x = int(round(param));

        assert(x <= min && x >= max);
        assert(max - min > 0);

        if (x % 2 == 0 && x != 0) {
            int shiftedDown = x - 1;
            if (shiftedDown >= min) {
                x = shiftedDown;
            }

            int shiftedUp = x + 1;
            if (shiftedUp <= max) {
                x = shiftedUp;
            }

            assert(false);
        }


        return remapParamInt(x, min, max);
    }

    double Module::fadeParam(double current, bool up, double step) {
        if (up) {
            current += step;
            if (current > MAX_PARAM) {
                current = MAX_PARAM;
            }
        } else {
            current -= step;
            if (current < MIN_PARAM) {
                current = MIN_PARAM;
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

#undef MIN_PARAM
#undef MAX_PARAM
