#include "syntheffect/synth.h"

#include <SDL.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <assert.h>
#include <boost/format.hpp>

#define MIN_PARAM 0
#define MAX_PARAM 1

namespace syntheffect {
    double Synth::remapParam(double param, double out_min, double out_max) {
        double in_min = MIN_PARAM;
        double in_max = MAX_PARAM; 

        return (param - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    int Synth::remapParamInt(double param, double out_min, double out_max) {
        return int(round(remapParam(param, out_min, out_max)));
    }

    int Synth::remapParamOdd(double param, double min, double max) {
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

    double Synth::fadeParam(double current, bool up, double step) {
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
};

#undef MIN_PARAM
#undef MAX_PARAM
