// forward declarations and headers which need to be handled first
// 2017 Dan Wilcox <danomatika@gmail.com>

// ignore everything in the private namespace
%ignore of::priv;

// needed for functions and return types
namespace std {
	%template(IntVector) std::vector<int>;
	%template(FloatVector) std::vector<float>;
	%template(StringVector) std::vector<std::string>;
	%template(UCharVector) std::vector<unsigned char>;
	%template(VideoDeviceVector) std::vector<ofVideoDevice>;
	%template(TextureVector) std::vector<ofTexture>;
};

// SWIG needs to know about boost::filesystem or it throws an error
namespace boost {
	namespace filesystem {}
}

// DIFF: std::filesystem::path is converted to string in the target language
%ignore std::filesystem::path;

// ----- ofConstants.h -----

// GL types used as OF arguments, etc so SWIG needs to know about them
typedef int GLint;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef float GLfloat;

%include "utils/ofConstants.h"

// ----- ofFbo.h -----

// need to forward declare these for ofFbo
%ignore ofBaseDraws;
class ofBaseDraws {};

%ignore ofBaseHasTexture;
class ofBaseHasTexture {};

%ignore ofBaseHasPixels;
class ofBaseHasPixels {};

// DIFF: ofFbo.h:
// DIFF:   ignoring const & copy constructor in favor of && constructor
%ignore ofFbo::ofFbo(ofFbo const &);

// DIFF:   setUseTexture & isUsingTexture are "irrelevant", so ignoring
%ignore ofFbo::setUseTexture;
%ignore ofFbo::isUsingTexture;

// DIFF:   ignoring setActiveDrawBufers() due to std::vector
%ignore setActiveDrawBuffers(const vector<int>& i);

// DIFF:   ignoring nested structs, not supported by SWIG
%ignore ofFbo::Settings;

%include "gl/ofFbo.h"

// ----- ofTexture.h -----

// DIFF: ofTexture.h: ignoring const & copy constructor in favor of && constructor
%ignore ofTexture::ofTexture(ofTexture const &);

%include "gl/ofTexture.h"

// ----- ofImage.h -----

// forward declare needed types
%ignore ofBaseImage_;
template<typename T> class ofBaseImage_ {};

// forward declare base template classes
%ignore ofBaseImage;
%ignore ofBaseFloatImage;
%ignore ofBaseShortImage;
#ifdef OF_SWIG_RENAME
	%template(BaseImage) ofBaseImage_<unsigned char>;
	%template(BaseFloatImage) ofBaseImage_<float>;
	%template(BaseShortImage) ofBaseImage_<unsigned short>;
#else
	%template(ofBaseImage) ofBaseImage_<unsigned char>;
	%template(ofBaseFloatImage) ofBaseImage_<float>;
	%template(ofBaseShortImage) ofBaseImage_<unsigned short>;
#endif

// DIFF: ofImage.h:
// DIFF:   ignore global helper functions
%ignore ofLoadImage;
%ignore ofSaveImage;
%ignore ofCloseFreeImage;

// DIFF:   ignoring ofPixels operator
%ignore ofImage_::operator ofPixels_<PixelType>&();

// DIFF:   ignoring const & copy constructor in favor of && constructor
%ignore ofImage_(const ofImage_<PixelType>&);

%include "graphics/ofImage.h"

// handle template implementations
#ifdef OF_SWIG_RENAME
	%template(Image) ofImage_<unsigned char>;
	%template(FloatImage) ofImage_<float>;
	%template(ShortImage) ofImage_<unsigned short>;
#else
	%template(ofImage) ofImage_<unsigned char>;
	%template(ofFloatImage) ofImage_<float>;
	%template(ofShortImage) ofImage_<unsigned short>;
#endif

// ----- ofBaseTypes.h -----

// DIFF: ofBaseTypes.h: ignore all abstract and base types
%ignore ofAbstractParameter;
%ignore ofBaseDraws;
%ignore ofBaseUpdates;
%ignore ofBaseHasTexture;
%ignore ofBaseHasTexturePlanes;

%ignore ofAbstractHasPixels;
%ignore ofBaseHasPixels_;
%ignore ofBaseHasPixels;
%ignore ofBaseHasFloatPixels;
%ignore ofBaseHasShortPixels;

%ignore ofAbstractImage;
%ignore ofBaseImage_;
%ignore ofBaseImage;
%ignore ofBaseFloatImage;
%ignore ofBaseShortImage;

%ignore ofBaseSoundInput;
%ignore ofBaseSoundOutput;

%ignore ofBaseVideo;
%ignore ofBaseVideoDraws;
%ignore ofBaseVideoGrabber;
%ignore ofBaseVideoPlayer;

%ignore ofBaseRenderer;
%ignore ofBaseGLRenderer;

%ignore ofBaseSerializer;
%ignore ofBaseFileSerializer;
%ignore ofBaseURLFileLoader;
%ignore ofBaseMaterial;

// still include header for derived classes
%include "types/ofBaseTypes.h"
