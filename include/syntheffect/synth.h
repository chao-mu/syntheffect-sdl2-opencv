#ifndef SYNTHEFFECT_SYNTH_H
#define SYNTHEFFECT_SYNTH_H
namespace syntheffect {
    class Synth {
        protected:
            double remapParam(double param, double out_min, double out_max);
            int remapParamInt(double param, double out_min, double out_max);
            int remapParamOdd(double param, double min, double max);
            double fadeParam(double current, bool up, double step);
    };
};
#endif
