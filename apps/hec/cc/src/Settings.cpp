#include "Settings.h"

ofSettings::ofSettings()
{
    parameters.setName("Parameters");

	parameters.add(colorMapIndex.set("colormap", 117, 0, 141));

    parameters.add(tolerance.set("tolerance", 0.0006f, 0.00005f, 0.1f));
	parameters.add(smoothing.set("smoothing", 7, 1, 30));
    parameters.add(blur_amount.set("blur amount", 7, 1, 30));

    parameters.add(erode_open_count.set("erosion open", 2, 0, 30));
	parameters.add(erode_close_count.set("erosion close", 1, 0, 30));
    parameters.add(dillate_count.set("dillate", 3, 0, 30));

	parameters.add(blur_amount2.set("blur amount 2", 3, 1, 30));
    parameters.add(erode_open_count2.set("erosion open 2", 2, 1, 30));
	parameters.add(erode_close_count2.set("erosion close 2", 1, 1, 30));
    parameters.add(dillate_count2.set("dillate 2", 3, 1, 30));

    //parameters.add(circle_size.set("circle size", 5, 1, 100));
    parameters.add(area_min.set("area min", 1500.0f, 500.0f, 50000.0f));
    parameters.add(area_max.set("area max", 5000000.0f, 5000.0f, 5000000.0f));
    //parameters.add(learingRate.set("learing rate", 0.99f, -1.0f, 1.0f));
	//parameters.add(gamma.set("gamma", 1.0, 0.0, 2.0));
	//parameters.add(amplify.set("amplify", 1.0, 0.0, 10.0));
	parameters.add(useConvexHull.set("use convex", false));

    parameters.add(nearClipping.set("near", 1000.0f, 0.0f, 8000.0f));
    parameters.add(farClipping.set("far",  8000.0f, 0.0f, 8000.0f));
}

ofParameterGroup& ofSettings::get_gui_parameters()
{
    return parameters;
}