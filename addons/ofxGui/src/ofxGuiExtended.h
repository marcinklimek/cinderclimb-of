#pragma once

#include "ofxGuiElement.h"
#include "ofxGuiGroup.h"
#include "ofxGuiSliderGroup.h"
#include "ofxGuiPanel.h"
#include "ofxGuiMenu.h"
#include "ofxGuiTabs.h"
#include "ofxGuiToggle.h"
#include "ofxGuiSlider.h"
#include "ofxGuiRangeSlider.h"
#include "ofxGuiButton.h"
#include "ofxGuiLabel.h"
#include "ofxGuiValuePlotter.h"
#include "ofxGuiFpsPlotter.h"
#include "ofxGuiFunctionPlotter.h"
#include "ofxGuiInputField.h"
#include "ofxGuiGraphics.h"
#include "ofxGuiZoomableGraphics.h"

#include "ofxDOM.h"

#include "ofxGuiDefaultConfig.h"
#include "ofxDOMFlexBoxLayout.h"
#include "ofxDOMBoxLayout.h"
#include "JsonConfigParser.h"

class ofxGui {
	public:
		ofxGui();
		~ofxGui();

		/// \brief Setup function initializing the GUI using the flexbox layout defined by ofxDOMFlexBoxLayout.
		/// This function has to be called before adding elements to the GUI.
		void setupFlexBoxLayout();

		/// \brief Template setup function initializing the GUI using the layout given by the template class.
		/// The setup function doesn't need to be called from the application if one wants to use
		/// the default ofxDOMBoxLayout, it will be executed automatically.
		/// This function has to be called before adding elements to the GUI.
		template<typename LayoutType = ofxDOMBoxLayout>
		void setup(){
			if(!setup_done){
				setup_done = true;
				document = std::make_unique<DOM::Document>();
				document->createLayout<LayoutType>(document.get());
			}else{
				ofLogError("ofxGui::setup()") << "Setup already done. This function needs to be called before getting the document or adding any elements to it.";
			}
		}

		/// \brief Get the root document of the GUI.
		/// \returns The root DOM::Document.
		DOM::Document* getDocument();

		/// \returns A parameter that determines if the gui is visible or not.
		ofParameter<bool>& getVisible();

		/// \brief Add a container to the document.
		/// A container is a collection of elements.
		/// \param name The container name.
		/// \param config The container configuration.
		/// \returns The GUI container element.
		ofxGuiContainer* addContainer(const std::string& name="", const ofJson& config = ofJson());

		/// \brief Add a container to the document.
		/// A container is a collection of elements.
		/// \param parameters A parameter group containing parameters that will be added to the container.
		/// \param config The container configuration.
		/// \returns The GUI container element.
		ofxGuiContainer* addContainer(const ofParameterGroup & parameters, const ofJson& config = ofJson());

		/// \brief Add a group to the document.
		/// A group is a container of elements with a header to minimize the group.
		/// \param name The group name.
		/// \param config The group configuration.
		/// \returns The GUI group element.
		ofxGuiGroup* addGroup(const std::string& name="", const ofJson& config = ofJson());

		/// \brief Add a group to the document.
		/// A group is a container of elements with a header to minimize the group.
		/// \param parameters A parameter group containing parameters that will be added to the group.
		/// \param config The group configuration.
		/// \returns The GUI group element.
		ofxGuiGroup* addGroup(const ofParameterGroup & parameters, const ofJson& config = ofJson());

		/// \brief Add a panel to the document.
		/// A panel is a container of elements with a header to save and load all child controls.
		/// \param name The panel name.
		/// \param config The panel configuration.
		/// \returns The GUI panel element.
		ofxGuiPanel* addPanel(const std::string& name="", const ofJson& config = ofJson());

		/// \brief Add a panel to the document.
		/// A panel is a container of elements with a header to save and load all child controls.
		/// \param parameters A parameter group containing parameters that will be added to the panel.
		/// \param config The panel configuration.
		/// \returns The GUI panel element.
		ofxGuiPanel* addPanel(const ofParameterGroup & parameters, const ofJson& config = ofJson());

		/// \brief Add a tabbed interface to the document.
		/// A tabbed interface is a container of other containers. For each added container a tab will be created.
		/// \param name The name of the tabbed interface (won't be displayed by default).
		/// \param config The tabbed interface configuration.
		/// \returns The tabbed interface element.
		ofxGuiTabs* addTabs(const std::string& name="", const ofJson& config = ofJson());

		/// \brief Set a config recursively for all elements of the GUI.
		/// \param config The configuration.
		void setConfig(const ofJson &config);

		/// \brief Add a parameter to the GUI without initializing a panel or group first.
		/// Creates a default panel if not already done.
		/// \param parameter The parameter that is going to be modifiable with the standard control type for the specific parameter type.
		template<typename T>
		void add(ofParameter<T>& parameter){
			if(!defaultPanel){
				defaultPanel = addPanel();
			}
			defaultPanel->add(parameter);
		}

		/// \brief Add parameters to the GUI without initializing a panel or group first.
		/// Creates a default panel if not already done.
		/// \param parameters The parameters that are going to be modifiable with the standard control type for each parameter type.
		template<typename T, typename... Args>
		void add(ofParameter<T>& parameter, Args... args) {
			getDocument()->blockLayout(true);
			add(parameter);
			add(args...) ;
			getDocument()->blockLayout(false);
		}

		ofxGuiContainer *addMenu(ofParameterGroup &content, const ofJson &config = ofJson());

		/// \brief Converts ofColor data type to a CSS-like rgba string.
		/// example: ofColor(255,0,0,127.5) -> "rgba(255,0,0,0.5)"
		/// \param color The color to be converted
		/// \returns The color in string rgba format.
		static std::string colorToString(const ofColor& color);

	private:
		std::unique_ptr<DOM::Document> document;

		bool setup_done;

		ofJson rootGroupConfig(const ofJson& config = ofJson());

		ofxGuiContainer* defaultPanel = nullptr;
};

