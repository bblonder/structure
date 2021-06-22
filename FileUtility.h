#import <Cocoa/Cocoa.h>
#import <ScreenSaver/ScreenSaver.h>

@interface FileUtility : NSObject {

}

+ (NSArray*) filesWithExtension:(NSString*) ext withArray:(NSArray*) array;
+ (const char*) chooseFilePath;

@end
