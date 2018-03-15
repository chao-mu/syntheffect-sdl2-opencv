#ifndef SYNTHEFFECT_MODULE_MODULE_H
#define SYNTHEFFECT_MODULE_MODULE_H
namespace syntheffect {
    namespace module {
        class Module {
            public:
                void start();
                void stop();
                void toggle();

            protected:
                double remapParam(double param, double out_min, double out_max, double in_min, double in_max);
                int remapParamInt(double param, double out_min, double out_max, double in_min, double in_max);
                int remapParamOdd(double param, double out_min, double out_max, double in_min, double in_max);
                double fadeParam(double current, bool up, double step, double out_in, double out_max);
                bool active_;
        };
    }
}
#endif
