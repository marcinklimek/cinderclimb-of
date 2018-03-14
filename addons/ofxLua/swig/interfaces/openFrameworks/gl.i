// gl folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// the following GL define values are pulled from glew.h & converted to base 10:

// DIFF: defined GL types:
// DIFF:   textures: OF_TEXTURE_LUMINANCE, OF_TEXTURE_RGB, & OF_TEXTURE_RGBA
#define OF_TEXTURE_LUMINANCE 6409    // 0x1909
#define OF_TEXTURE_RGB 6407          // 0x1907
#define OF_TEXTURE_RGBA 6408         // 0x1908

// DIFF:   mipmap filter: OF_NEAREST, OF_LINEAR
#define OF_NEAREST 9728              // 0x2600
#define OF_LINEAR 9729               // 0x2601

// DIFF:   shader: OF_FRAGMENT_SHADER, OF_VERTEX_SHADER
#define OF_FRAGMENT_SHADER 35632     // 0x8B30
#define OF_VERTEX_SHADER 35633       // 0x8B31

// DIFF:   texture wrap: OF_CLAMP_TO_EDGE, OF_CLAMP_TO_BORDER, OF_REPEAT, OF_MIRRORED_REPEAT
#define OF_CLAMP_TO_EDGE 33071       // 0x812F
#ifndef TARGET_OPENGLES
	#define OF_CLAMP_TO_BORDER 33069 // 0x812D
#endif
#define OF_REPEAT 10497              // 0x2901
#define OF_MIRRORED_REPEAT 33648     // 0x8370

// ----- ofBufferObject.h -----

%include "gl/ofBufferObject.h"

// ----- ofGLProgrammableRenderer.h -----

// not needed

// ----- ofFbo.h -----

// handled in main.i

// ----- ofGLRenderer.h -----

// not needed

// ----- ofGLUtils.h -----

// DIFF: ofGLUtils.h: ignoring ofGetGLRenderer()
%ignore ofGetGLRenderer();

// manually rename these otherwise the initial G in GL ends up lowercase
#ifdef OF_SWIG_RENAME
	%rename ofGLCheckExtension GLCheckExtension;
	%rename ofGLSLVersionFromGL GLSLVersionFromGL;
	%rename ofGLSupportedExtensions GLSupportedExtensions;
	%rename ofGLSupportsNPOTTextures GLSupportsNPOTTextures;
#endif

// ignore extra GL defines
%rename($ignore, regextarget=1) "GL_$";

%include "gl/ofGLUtils.h"

// ----- ofLight.h -----

// DIFF: ofLight.h: ignoring nested struct, not supported by SWIG
%ignore ofLight::Data;

%include "gl/ofLight.h"

// ----- ofMaterial.h -----

// DIFF: ofMaterial.h: ignoring nested struct, not supported by SWIG
%ignore ofMaterial::Data;
%ignore ofMaterial::getData() const;
%ignore ofMaterial::setData(const ofMaterial::Data &);

%include "gl/ofMaterial.h"

// ----- ofShader.h -----

// DIFF: ofShader.h: ignoring const & copy constructor in favor of && constructor
%ignore ofShader::ofShader(ofShader const &);

// ignore destructor
%ignore ofShader::~ofShader;

%include "gl/ofShader.h"

// TODO: ofShader.h: remove custom destructor if bug fixed in future OF versions
// DIFF: ofShader.h: custom destructor so shaders are unbound before deletion
%rename("%s") ofShader::~ofShader; // unignore
%extend ofShader {
public:
	~ofShader() {
		$self->end();
		delete $self;
	}
};

// ----- ofTexture.h -----

// handled in main.i

// ----- ofVbo.h -----

%include "gl/ofVbo.h"

// ----- ofVboMesh.h -----

%include "gl/ofVboMesh.h"
