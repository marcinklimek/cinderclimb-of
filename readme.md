What is it?
===========

This is an environment for programming climbing wall activities but you can use it with any wall. Based on OpenFrameworks library, Kinect 2 camera and projector.

Activities are programmed in an internal Lua scripting language.


[Cinder Climb](https://klimek.link/blog/cc/)
------------

My project for a climbing wall. Although in principle a climbing wall is not necessary.
The first recorded tests took place on March 11, 2016. A very simple and reasonably effective solution.


[![First version](https://img.youtube.com/vi/0wNuXHkMRwQ/0.jpg)](http://www.youtube.com/watch?v=0wNuXHkMRwQ)


Latest version. OpenFrameworks based solution. Activities programmed in Lua.

[![OpenFrameworks version](https://img.youtube.com/vi/f2_wTw7pFhY/0.jpg)](http://www.youtube.com/watch?v=f2_wTw7pFhY)

TODO
====


Types of activities, for people with disabilities:
- extinguishing points vertically, unilateral linear movement and alternating
- extinguishing segments of the displayed picture, moving around the whole wall
- sorting elements by categories (ascending, descending) - screen version
- Clock exercise - following the clock hands (additional version for the screen)

Types of activity, for healthy people:

- water and fire - running away from water flooding us from below and/or fire from above
- erasing the image with one's own body
- catching eggs, the higher the more points
- smearing with the whole body - the effect of smearing with poster paints, for children who have just started climbing. The first steps in climbing, getting familiar with holds
- displaying a shape on the wall - you have to position yourself according to it and persevere for a while
- jumping frog - only one hold is visible and the participant jumps to the next hold


Other
=====

Must read
---------

* https://github.com/luarocks/lua-style-guide
* http://lua-users.org/wiki/LuaStyleGuide
* https://www.lua.org/gems/sample.pdf
* http://notebook.kulchenko.com/programming/lua-good-different-bad-and-ugly-parts

Todo in lua
-----------

* https://github.com/mpeterv/luacheck
* https://github.com/danoli3/ofxTexturePacker

Tools
-----

* https://www.defold.com



Compile under VS
---------------

* https://www.youtube.com/results?search_query=automatic+camera+calibration+geometry
* http://unanancyowen.com/en/kinect-v2-coordinate-system-mapping/
* https://github.com/LightBuzz/Vitruvius
* https://github.com/labatrockwell/openTSPS

* https://mediatech.aalto.fi/~rakajast/Publs/kajastila_paper_chi_wip_2014_camera_ready.pdf
* http://www.augmentedclimbing.com/

* https://vimeo.com/123021654
* https://vimeo.com/127154814
* https://vimeo.com/89390488


* https://github.com/gumbykid/Climbing-Games

* https://www.reddit.com/r/climbing/comments/3npg5m/speed_bouldering_game_with_a_projector/


* https://forum.processing.org/one/topic/new-blob-detection-and-analysis-image-labelling-and-segmentation-library-for-processing.html

* https://www.youtube.com/watch?v=VKLDGCppInw&t=2946s

* scoring https://github.com/search?p=7&q=Climbing&type=Repositories&utf8=%E2%9C%93



Games for the wall
-------------------

* flood
    * http://www.patrickmatte.com/stuff/physicsLiquid/
    * http://gamedevelopment.tutsplus.com/tutorials/make-a-splash-with-dynamic-2d-water-effects--gamedev-236
    * https://sourceforge.net/projects/syfluid/
    * check! https://bitbucket.org/quilime/cindertop
* erasing the background
* another climber adds 2 holds on to the problem
* eggs
* catch colors (large color spots to catch)
* smudging a photo based on Fluid2DTexture
* displaying the shape on the wall, you have to position yourself according to it and persist for a while (http://phylopic.org/image/browse/)
* jumping frog - you can only see one catch and the frog jumps to the next catch




R&D
===

Inspirations
-------------
* https://www.youtube.com/watch?v=OGoZktCzMS4   
* https://github.com/smmankad/float
* https://www.youtube.com/watch?v=O8tT0GXTOPo


Body center of mass
-------------------
* https://www.reddit.com/r/dataisbeautiful/comments/4rup10/i_did_a_simple_mechanical_analysis_of_that/
* http://stackoverflow.com/questions/21973582/calculating-center-of-mass-of-body-being-tracked-using-kinect


Other
-----

* https://www.researchgate.net/publication/256822482_Part-based_motion_descriptor_image_for_human_action_recognition
* https://www.researchgate.net/publication/222182932_Duygulu_P_Histogram_of_Oriented_Rectangles_A_New_Pose_Descriptor_for_Human_Action_Recognition_Image_and_Vision_Computing_2710_1515-1526
* http://answers.opencv.org/question/35912/how-to-detect-individual-body-parts-in-an-image/
* https://people.eecs.berkeley.edu/~rbg/latent/
* https://github.com/rbgirshick/voc-dpm
* http://www.robots.ox.ac.uk/~vgg/data/stickmen/
* http://vision.ucsd.edu/sites/default/files/dollarBMVC09ChnFtrs_0.pdf  !! ciekawe
* http://www.ics.uci.edu/~dramanan/papers/pose_pami.pdf - sciagniete
* https://www.mpi-inf.mpg.de/departments/computer-vision-and-multimodal-computing/research/people-detection-pose-estimation-and-tracking/pictorial-structures-revisited-people-dectection-and-articulated-pose-estimation/
* https://www.researchgate.net/publication/308960836_Video_Pose_Estimation_with_Global_Motion_Cues

* http://jaesik.info/publications/depthups/index.html
* http://jaesik.info/publications/depthups/tip14_paper.pdf


Camera
------

* http://www.wumpus-cave.net/2015/11/06/low-latency-fpv-streaming-with-the-raspberry-pi/
* https://dev.px4.io

* https://pixhawk.org/peripherals/rangefinder  
* lidar? ->  https://www.parallax.com/product/28057


Problems
========

GLM
---

After the update of GLM lib, the templated function

		template<typename genType>
		GLM_FUNC_QUALIFIER genType fma(genType const& a, genType const& b, genType const& c)
		{
			return a * b + c;
		}

has to be enabled, otherwise unresolved symbols will appear.


ofxLuaBind
----------

Before generating bindings with SWIG, In the file ofUtils.h:

template <typename VAList>
auto  ofVAArgsToString(const char * format, VAList args)
    -> typename std::enable_if<std::is_same<va_list, VAList>::value, std::string>::type
{
    return ofVAListToString(format, args);
}

comment it out. Otherwise SWIG will fail


Generating the bindings:

-gen.bat-
rem choco install swig

swig -v -c++ -lua -O -Ic:/workspace/climbing/of/libs/openFrameworks -DMODULE_NAME=of -DOF_SWIG_RENAME -o lua/desktop/ofBindings.cpp openFrameworks.i
swig -v -c++ -lua -O -debug-lsymbols -Ic:/workspace/climbing/of/libs/openFrameworks -DMODULE_NAME=of -DOF_SWIG_RENAME -o lua/desktop/ofBindings.cpp openFrameworks.i >of_symbols.txt

swig -v -c++ -lua -O -Ic:\workspace\climbing\of\libs\glm\include\glm -DMODULE_NAME=of -DOF_SWIG_RENAME -o lua/glmBindings.cpp glm.i
swig -v -c++ -lua -O -debug-lsymbols -Ic:\workspace\climbing\of\libs\glm\include\glm -DMODULE_NAME=of -DOF_SWIG_RENAME -o lua/desktop/glmBindings.cpp glm.i >glm_symbols.txt

swig -v -c++ -lua -external-runtime lua/desktop/ofBindings.h
