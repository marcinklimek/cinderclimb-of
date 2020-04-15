#include "Settings.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

ofSettings::ofSettings()
{
    color_preview_pos.set(spacing, spacing + preview_H + spacing, DEPTH_WIDTH, DEPTH_HEIGHT);

    parameters.setName("Parameters");

	parameters.add(colorMapIndex.set("colormap", 117, 0, 141));

    parameters.add(tolerance.set("tolerance", 0.0006f, 0.00005f, 0.1f));
	parameters.add(smoothing.set("smoothing", 7, 1, 30));
    parameters.add(blur_amount.set("blur amount", 0, 0, 30));

    parameters.add(erode_open_count.set("erosion open", 0, 0, 30));
	parameters.add(erode_close_count.set("erosion close", 0, 0, 30));
    parameters.add(dillate_count.set("dillate", 0, 0, 30));

	parameters.add(blur_amount2.set("blur amount 2", 0, 0, 30));
    parameters.add(erode_open_count2.set("erosion open 2", 1, 0, 30));
	parameters.add(erode_close_count2.set("erosion close 2", 0, 0, 30));
    parameters.add(dillate_count2.set("dillate 2", 0, 0, 30));

    parameters.add(area_min.set("area min", 500.0f, 500.0f, 50000.0f));
    parameters.add(area_max.set("area max", 5000000.0f, 5000.0f, 5000000.0f));
    
    parameters.add(thresholdValue.set("Threshold Value", 10, 0, 255));

    parameters.add(nearClipping.set("near", 0000.0f, 0.0f, 8000.0f));
    parameters.add(farClipping.set("far",  8000.0f, 0.0f, 8000.0f));

    parameters.add(resetBackground.set("reset Background", true));
    parameters.add(resetBackgroundTime.set("reset Background time", 5, 1, 30));
    parameters.add(epsilon.set("epsilon", 100, 0, 10000));

	scan_for_scripts();
}

void ofSettings::scan_for_scripts()
{
    // scripts to run

	const string path = "scripts";
	ofDirectory dir(path);

	dir.allowExt("lua");
	dir.listDir();

	scripts.clear();
	scripts.emplace_back("scripts/start.lua");

	for (auto i = 0; i < dir.size(); i++) 
	{
		const auto name = dir.getPath(i);
		if ( boost::starts_with( name, "scripts\\game_" ) )
		{
			scripts.push_back( name );
		}
	}

	std::sort(scripts.begin(), scripts.end());

	parameters.add(currentScript.set("script", 0, 0, scripts.size()-1) );
}

string ofSettings::get_script()
{
	if (currentScript > 0 && currentScript < scripts.size())
		return scripts[currentScript];

	return scripts[0];
}


ofParameterGroup& ofSettings::get_gui_parameters()
{
    return parameters;
}