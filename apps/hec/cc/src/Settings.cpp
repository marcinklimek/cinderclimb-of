#include "Settings.h"

ofSettings::ofSettings()
{
    parameters.setName("Parameters");

    parameters.add(threshold.set("threshold", 0, 0, 255));
    parameters.add(blur_amount.set("blur amount", 3, 1, 30));
    parameters.add(brightness.set("brightness", 0.28f, 0.0f, 1.0f));
    parameters.add(contrast.set("contrast", 0.28f, 0.0f, 1.0f));
    parameters.add(erode_open_count.set("erosion open", 1, 1, 30));
	parameters.add(erode_close_count.set("erosion close", 1, 1, 30));
    parameters.add(dillate_count.set("dillate", 1, 1, 30));
    parameters.add(circle_size.set("circle size", 5, 1, 100));
    parameters.add(area_min.set("area min", 10.0f, 10.0f, 5000.0f));
    parameters.add(area_max.set("area max", 5000000.0f, 5000.0f, 5000000.0f));
    parameters.add(learingRate.set("learing rate", 0.99f, -1.0f, 1.0f));
	parameters.add(gamma.set("gamma", 1.0, 0.0, 2.0));
	parameters.add(amplify.set("amplify", 1.0, 0.0, 10.0));
	parameters.add(useConvexHull.set("use convex", false));
	
   
}

ofParameterGroup& ofSettings::get_gui_parameters()
{
    return parameters;
}