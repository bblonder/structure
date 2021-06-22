#import <Cocoa/Cocoa.h>
#include "RenderObject.h"
#include "StructureGLString.h"

@interface StructureOpenGLView : NSOpenGLView {
	RenderObject *renderObject;
	StructureGLString *titleString, *authorString, *infoString, *chainString;
	
	float fadeAlpha, percentDone;
	bool renderObjectExists;
	int renderMode;
}

- (void) chooseNewPDB;
- (void) setFadeAlpha:(float) fa;
- (void) setPercentDone:(float) pd;

- (NSAttributedString*) attributedStringWithFont:(NSFont*)f color:(NSColor*)c text:(std::string)t;
- (StructureGLString*) glStringWithString:(NSAttributedString*)s;
- (bool) captureView:(NSString*) path;

@end
