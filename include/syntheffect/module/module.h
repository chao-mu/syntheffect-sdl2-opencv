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
                double remapParam(double param, double out_min, double out_max);
                int remapParamInt(double param, double out_min, double out_max);
                int remapParamOdd(double param, double min, double max);
                double fadeParam(double current, bool up, double step);
                bool active_;
        };
    };
};
#endif
