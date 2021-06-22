#import "StructureOpenGLView.h"
#include "Color.h"
#include "RandomUtil.h"
#import "PDBDownload.h"
#import "FileUtility.h"
#include "StringHelper.h"

@implementation StructureOpenGLView

- (void) drawRect:(NSRect)rect
{
	[[self openGLContext] makeCurrentContext];
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (renderObjectExists)
	{
		[self reshape];
		
		renderObject->render(percentDone, renderMode);
		
		GLint matrixMode;
		int width = [self bounds].size.width;
		int height = [self bounds].size.height;
		glGetIntegerv (GL_MATRIX_MODE, &matrixMode);
		glMatrixMode (GL_PROJECTION);
			glPushMatrix();
				glLoadIdentity ();
				glMatrixMode (GL_MODELVIEW);
				glPushMatrix();
					glLoadIdentity ();
					glScalef (2.0f / width, -2.0f /  height, 1.0f);
					glTranslatef (-width / 2.0f, -height / 2.0f, 0.0f);
					glColor4f (1.0f, 1.0f, 1.0f, 1.0f); // surprisingly necessary
					
					// setup is done - let's draw the strings!
					
					// draw from the top
					float currentHeight = 0;
					
					[titleString drawAtPoint:NSMakePoint (0, currentHeight)];
					currentHeight += [titleString texSize].height;
					
					if ([[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] boolForKey:@"FullTextualAnnotation"])
					{
						[authorString drawAtPoint:NSMakePoint (0, currentHeight)];
						currentHeight += [authorString texSize].height;
						
						[infoString drawAtPoint:NSMakePoint (0, currentHeight)];
						currentHeight += [infoString texSize].height;
						
						// draw from the bottom
						[chainString drawAtPoint:NSMakePoint (0, [self bounds].size.height - [chainString texSize].height)];
					}
					
					
					glPopMatrix(); // GL_MODELVIEW
			glMatrixMode (GL_PROJECTION);
		glPopMatrix();
		glMatrixMode (matrixMode);
		glDisable (GL_TEXTURE_RECTANGLE_EXT);
		glEnable (GL_DEPTH_TEST);
		
		// now do the fade
		RenderObject::fade(fadeAlpha);
	}
	else
	{
		NSLog(@"drawRect failed: no renderObject");
	}
	
	glFlush();
}

- (void) prepareOpenGL
{
    GLint swapInt = 1;

    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
	
	RenderObject::initializeGL();
	
	fadeAlpha = 1.0;
	percentDone = 0;
}

- (void)reshape
{
	[[self openGLContext] makeCurrentContext];
	
	NSRect bounds = [self bounds];
	if (renderObject != NULL)
	{
		renderObject->setProjection(bounds.size.width, bounds.size.height);
	}
	else
	{
		NSLog(@"reshape failed: no renderObject");
	}
}

- (void) chooseNewPDB
{	
	[[self openGLContext] makeCurrentContext];
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();

	if (renderObject != NULL)
	{
		delete renderObject;
		renderObjectExists = false;
		
		[titleString release];
		[authorString release];
		[infoString release];
		[chainString release];
	}
	
	renderObject = new RenderObject([FileUtility chooseFilePath]);
	renderObjectExists = true;

	Molecule *m = renderObject->m;
	
	renderMode = [[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] integerForKey:@"RenderMode"];
	
	if ([[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] boolForKey:@"FullTextualAnnotation"])
	{
		titleString = [self glStringWithString:[self attributedStringWithFont:
								[NSFont fontWithName: @"Lucida Grande" size: 18.0f]
			color:				[NSColor colorWithDeviceRed:1.0 green:1.0 blue:1.0 alpha:1.0]
			text:				m->title + " (" + m->idCode + ")"
			]];	
	}
	else
	{
		titleString = [self glStringWithString:[self attributedStringWithFont:
								[NSFont fontWithName: @"Lucida Grande" size: 18.0f]
			color:				[NSColor colorWithDeviceRed:1.0 green:1.0 blue:1.0 alpha:1.0]
			text:				m->idCode
			]];
	}
	

		
	authorString = [self glStringWithString:[self attributedStringWithFont:
							[NSFont fontWithName: @"Lucida Grande" size: 14.0f]
		color:				[NSColor colorWithDeviceRed:1.0 green:1.0 blue:1.0 alpha:1.0]
		text:				 m->date + " - " + m->authors
		]];
		
	infoString = [self glStringWithString:[self attributedStringWithFont:
							[NSFont fontWithName: @"Lucida Grande" size: 12.0f]
		color:				[NSColor colorWithDeviceRed:1.0 green:1.0 blue:1.0 alpha:0.6]
		text:				"Method:\t" + m->experiment + " " + m->resolution + "\nSource:\t" + m->organism + "\nType:\t" + m->classification
		]];
		
	NSMutableAttributedString *chainText = [[NSMutableAttributedString alloc] initWithString:@""];
	
	for (std::map<std::string,std::string>::iterator it = m->compounds.begin(); it != m->compounds.end(); ++it)
	{
		Point4 color = m->colorInfo.getChainColor((*it).first);
		
		[chainText appendAttributedString:[self attributedStringWithFont:
								[NSFont fontWithName: @"Lucida Grande" size: 12.0f]
			color:				[NSColor colorWithDeviceRed:color.x green:color.y blue:color.z alpha:1.0]
			text:				/*((it == m->compounds.begin()) ? "" : "\n") + */removeWhitespace((*it).first, " ", " ") + "\t" + (*it).second + "\n"
		]];
	}
		
	chainString = [self glStringWithString:chainText];
}

- (NSAttributedString*) attributedStringWithFont:(NSFont*)f color:(NSColor*)c text:(std::string)t
{
	NSMutableDictionary *attribs = [NSMutableDictionary dictionary];
    [attribs setObject:f forKey: NSFontAttributeName];
	[attribs setObject:c forKey: NSForegroundColorAttributeName];
	NSString *text = [NSString stringWithCString:t.c_str() encoding:[NSString defaultCStringEncoding]];
	return [[NSAttributedString alloc] initWithString:text attributes:attribs];
}

- (StructureGLString*) glStringWithString:(NSAttributedString*)s
{
	StructureGLString *glString = [[StructureGLString alloc] initWithAttributedString:s];
	[glString setBoxColor:[NSColor clearColor]];
	[glString setBorderColor:[NSColor clearColor]];
	NSSize frame = [self bounds].size;
	frame.width -= 10.0;
	[glString useStaticFrame:frame];
	
	return glString;
}

- (void) setFadeAlpha:(float) fa
{
	fadeAlpha = fa;
}

- (void) setPercentDone:(float) pd
{
	percentDone = pd;
}

- (bool) captureView:(NSString*) path
{
    [self drawRect:[self bounds]];

    int width = (int) [self bounds].size.width;
    int height = (int) [self bounds].size.height;
    void* buffer = malloc(width*height*4);

    glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    
    unsigned char* temp[5];
    temp[0] = (unsigned char*) buffer;

    NSBitmapImageRep* bitmap = [[NSBitmapImageRep alloc] 
        initWithBitmapDataPlanes:temp 
        pixelsWide:width 
        pixelsHigh:height 
        bitsPerSample:8 
        samplesPerPixel:3 
        hasAlpha:NO 
        isPlanar:NO 
        colorSpaceName:NSCalibratedRGBColorSpace 
        bytesPerRow:width*4 
        bitsPerPixel:32];
    
    NSImage* image = [[NSImage alloc] init];
    [image addRepresentation:bitmap];
    [image setFlipped:YES];
    [image lockFocusOnRepresentation:bitmap];
    [image unlockFocus];
    
    free(buffer);
	
	return [[image TIFFRepresentation] writeToFile:path atomically:YES];
}

- (BOOL) acceptsFirstResponder
{
     return YES;
}

- (void)keyDown:(NSEvent *)theEvent
{
	unichar keyChar = [[theEvent charactersIgnoringModifiers] characterAtIndex:0];
	
	NSString *pdbID = [NSString stringWithCString:renderObject->m->idCode.c_str() encoding:[NSString defaultCStringEncoding]];
	
	if (keyChar == '1')
	{
		[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[@"http://www.rcsb.org/pdb/explore/explore.do?structureId=" stringByAppendingString:pdbID]]];
	}
	else if (keyChar == '2')
	{
		NSString *path = [[[@"~/Desktop" stringByExpandingTildeInPath] stringByAppendingPathComponent:pdbID] stringByAppendingPathExtension:@"tif"];
		[self captureView:path];
	}
	else if (keyChar == '3')
	{
		renderMode = (renderMode + 1);
		if (renderMode > 2) renderMode = 0;
	}
	else if (keyChar == '4')
	{
		[self chooseNewPDB];
	}
	else
	{
		[super keyDown:theEvent];
	}
}


@end