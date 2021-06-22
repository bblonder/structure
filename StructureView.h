#import <ScreenSaver/ScreenSaver.h>
#import "StructureOpenGLView.h"


@interface StructureView : ScreenSaverView 
{
	StructureOpenGLView *glView;
	int framesPerSecond;
	int frameCount;
	
    IBOutlet NSWindow *configSheet;
    IBOutlet id controlCacheSize;
    IBOutlet id controlDisplayPeriod;
    IBOutlet id controlEnableInternetAccess;
    IBOutlet id controlErrorText;
    IBOutlet id controlFullTextualAnnotation;
	IBOutlet id controlOnlyLoadLocalFiles;
	IBOutlet id controlRenderMode;
}

- (IBAction)cancelClick:(id)sender;
- (IBAction)okClick:(id)sender;
- (IBAction)updateClick:(id)sender;

@end
