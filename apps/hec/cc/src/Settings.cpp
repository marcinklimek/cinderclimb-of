#include "Settings.h"

ofSettings::ofSettings()
{
    parameters.setName("Parameters");

	parameters.add(colorMapIndex.set("colormap", 117, 0, 141));

    parameters.add(tolerance.set("tolerance", 0.0006f, 0.00005f, 0.1f));
	parameters.add(smoothing.set("smoothing", 7, 1, 30));
    parameters.add(blur_amount.set("blur amount", 0, 0, 30));

    parameters.add(erode_open_count.set("erosion open", 0, 0, 30));
	parameters.add(erode_close_count.set("erosion close", 0, 0, 30));
    parameters.add(dillate_count.set("dillate", 0, 0, 30));

	parameters.add(blur_amount2.set("blur amount 2", 0, 0, 30));
    parameters.add(erode_open_count2.set("erosion open 2", 0, 0, 30));
	parameters.add(erode_close_count2.set("erosion close 2", 0, 0, 30));
    parameters.add(dillate_count2.set("dillate 2", 0, 0, 30));

    parameters.add(area_min.set("area min", 5500.0f, 500.0f, 50000.0f));
    parameters.add(area_max.set("area max", 5000000.0f, 5000.0f, 5000000.0f));
    
    parameters.add(thresholdValue.set("Threshold Value", 10, 0, 255));

    parameters.add(nearClipping.set("near", 1000.0f, 0.0f, 8000.0f));
    parameters.add(farClipping.set("far",  8000.0f, 0.0f, 8000.0f));

    parameters.add(resetBackground.set("reset Background", true));
    parameters.add(resetBackgroundTime.set("reset Background time", 10, 1, 30));
    parameters.add(epsilon.set("epsilon", 500, 0, 10000));
    
    
    
}

ofParameterGroup& ofSettings::get_gui_parameters()
{
    return parameters;
}