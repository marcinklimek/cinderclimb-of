#include "Settings.h"


ofSettings::ofSettings()
{
    parameters.setName("Parameters");

    parameters.add(threshold.set("threshold", 0, 0, 255));
    parameters.add(blur_amount.set("blur amount", 10, 1, 30));
    parameters.add(brightness.set("brightness", 0.28f, 0.0f, 1.0f));
    parameters.add(contrast.set("contrast", 0.28f, 0.0f, 1.0f));
    parameters.add(erode_count.set("erosion", 8, 1, 30));
    parameters.add(dillate_count.set("dillate", 15, 1, 30));
    parameters.add(circle_size.set("circle size", 5, 1, 100));
    parameters.add(area_min.set("area min", 1500.0f, 10.0f, 5000.0f));
    parameters.add(area_max.set("area max", 150000.0f, 5000.0f, 500000.0f));
    parameters.add(learingRate.set("learing rate", -1.0f, -1.0f, 1.0f));
   
}

ofParameterGroup& ofSettings::get_gui_parameters()
{
    return parameters;
}