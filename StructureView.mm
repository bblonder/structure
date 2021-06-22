#import "StructureView.h"
#include "RandomUtil.h"
#import "PDBDownload.h"

@implementation StructureView

- (id)initWithFrame:(NSRect)frame isPreview:(BOOL)isPreview
{
    self = [super initWithFrame:frame isPreview:isPreview];

	// Register our default values
	[[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] registerDefaults:[NSDictionary dictionaryWithObjectsAndKeys:
		@"30", @"DisplayPeriod", 
		@"100", @"CacheSize",
		@"1", @"FullTextualAnnotation",
		@"1", @"EnableInternetAccess",
		@"0", @"OnlyLoadLocalFiles",
		@"1", @"RenderMode",
		nil
		]];

	srand(time(NULL));
	
	framesPerSecond = 30;
	
	glView = [[StructureOpenGLView alloc] initWithFrame:NSMakeRect(0, 0, frame.size.width, frame.size.height) ]; 
	[glView prepareOpenGL];

	[self setAutoresizesSubviews:YES];
	[self addSubview:glView];
	
	[glView chooseNewPDB];
	
	[self setAnimationTimeInterval:1.0/framesPerSecond];

    return self;
}

- (void)startAnimation
{
    [super startAnimation];
}

- (void)stopAnimation
{
    [super stopAnimation];
}

- (void)drawRect:(NSRect)rect
{
    [super drawRect:rect];
}

- (void)animateOneFrame
{
	int fadePeriod = 2;
	int showPeriod = [[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] integerForKey:@"DisplayPeriod"];
	float alpha;
	
	if (1.0 * frameCount / framesPerSecond < fadePeriod)
	{
		alpha = 1.0 - 1.0 * frameCount / framesPerSecond;
	}
	else if (1.0 * frameCount / framesPerSecond > (showPeriod - fadePeriod))
	{
		alpha = 1.0 - 1.0 * ((showPeriod * framesPerSecond) - frameCount) / framesPerSecond;
	}
	else
	{
		alpha = 0.0;
	}
	
	[glView setFadeAlpha:alpha];
	[glView setPercentDone:1.0*frameCount/(showPeriod * framesPerSecond)];

	[glView setNeedsDisplay:YES];
	
	frameCount++;
	if (frameCount >= showPeriod * framesPerSecond)
	{
		frameCount = 0;
		[glView chooseNewPDB];
		
		// download
		float rf = randomFloat(0,1);
		if (rf < 0.5 && [[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] boolForKey:@"EnableInternetAccess"])
		{
			[[[PDBDownload alloc] init] downloadNewPDB];
		}
	}
	
    return;
}

- (BOOL) hasConfigureSheet
{
    return YES;
}

- (NSWindow*)configureSheet
{ 
	ScreenSaverDefaults *defaults;

	defaults = [ScreenSaverDefaults defaultsForModuleWithName:@"Structure"];

	if (!configSheet)
	{
		if (![NSBundle loadNibNamed:@"ConfigureSheet" owner:self]) 
		{
			NSLog( @"Failed to load configure sheet." );
			NSBeep();
		}
	}
	
	// needs setIntValue not setIntegerValue for 10.4 compatibility...
	[controlDisplayPeriod setIntValue:[defaults integerForKey:@"DisplayPeriod"]];
	[controlCacheSize setIntValue:[defaults integerForKey:@"CacheSize"]];
	[controlEnableInternetAccess setIntValue:[defaults boolForKey:@"EnableInternetAccess"]];
	[controlFullTextualAnnotation setIntValue:[defaults boolForKey:@"FullTextualAnnotation"]];
	[controlOnlyLoadLocalFiles setIntValue:[defaults boolForKey:@"OnlyLoadLocalFiles"]];
	[controlRenderMode selectItemAtIndex:[defaults integerForKey:@"RenderMode"]];
	
	return configSheet;
}

- (IBAction)updateClick:(id)sender
{
	if ([[[PDBDownload alloc] init] updateList])
	{
		NSRunAlertPanel(@"Download successful", @"The master list of available structures on the PDB archive has been updated.", nil, nil, nil);
	}
	else
	{
		NSRunAlertPanel(@"Download unsuccessful", @"The master list could not be downloaded. Please try again later.", nil, nil, nil);
	}
}

- (IBAction)cancelClick:(id)sender
{
	[[NSApplication sharedApplication] endSheet:configSheet];
}

- (IBAction)okClick:(id)sender
{
	ScreenSaverDefaults *defaults = [ScreenSaverDefaults defaultsForModuleWithName:@"Structure"];

	// validate all input
	bool okay = true;
	NSString* errorString = @"";
	
	int displayPeriod = [controlDisplayPeriod intValue];
	if (displayPeriod < 5)
	{
		errorString = @"Display period must be at least 5 seconds.";
		okay = false;
	}
	
	int cacheSize = [controlCacheSize intValue];
	if (cacheSize < 1)
	{
		errorString = @"Cache size must be at least one.";
		okay = false;
	}
	
	[controlErrorText setStringValue:errorString];
	
	if (okay)
	{
		[defaults setInteger:displayPeriod forKey:@"DisplayPeriod"];
		[defaults setInteger:cacheSize forKey:@"CacheSize"];
		[defaults setInteger:[controlFullTextualAnnotation state] forKey:@"FullTextualAnnotation"];
		[defaults setInteger:[controlEnableInternetAccess state] forKey:@"EnableInternetAccess"];
		[defaults setInteger:[controlOnlyLoadLocalFiles state] forKey:@"OnlyLoadLocalFiles"];
		[defaults setInteger:[controlRenderMode indexOfSelectedItem] forKey:@"RenderMode"];
		 
		// Save the settings to disk
		[defaults synchronize];
		 
		// Close the sheet
		[[NSApplication sharedApplication] endSheet:configSheet];
		
		// trigger a new structure selection
		frameCount = [[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] integerForKey:@"DisplayPeriod"] * framesPerSecond;
	}
}

@end
