#import "FileUtility.h"
#include "RandomUtil.h"

@implementation FileUtility

+ (NSArray*) filesWithExtension:(NSString*) ext withArray:(NSArray*) array
{
	NSString *s;
	NSMutableArray *ma = [[NSMutableArray alloc] initWithCapacity:100];

	NSEnumerator *e = [array objectEnumerator];
	while (s = [e nextObject])
	{
		if ([[[s uppercaseString] pathExtension] isEqualToString:ext])
		{
			[ma addObject:s];
		}
		else
		{

		}
	}
	
	return ma;
}

+ (const char*) chooseFilePath
{
	NSString* internalResourcePath = [[NSBundle bundleForClass:NSClassFromString(@"StructureOpenGLView")] resourcePath];
	NSArray* internalContents = [self filesWithExtension:@"PDB" withArray:[[NSFileManager defaultManager] directoryContentsAtPath:internalResourcePath]];
	int numInternalItems = [internalContents count];

	if ([[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] boolForKey:@"OnlyLoadLocalFiles"])
	{
		numInternalItems = 0;
	}

	NSString* externalResourcePath = [NSHomeDirectory() stringByAppendingPathComponent:@"Library/Structure"];
	NSArray* externalContents = [self filesWithExtension:@"PDB" withArray:[[NSFileManager defaultManager] directoryContentsAtPath:externalResourcePath]];
	int numExternalItems = [externalContents count];
	
	NSString *filePath = @"";
	
	float rn = randomFloat(0, 1);
	float wa = ((float) numInternalItems) / ((float) (numInternalItems + numExternalItems));

	if (rn < wa && numInternalItems > 0)
	{
		filePath = [internalResourcePath stringByAppendingPathComponent:[internalContents objectAtIndex:randomInt(0, numInternalItems - 1)]];
	}
	else if (rn >= wa && numExternalItems > 0)
	{
		filePath = [externalResourcePath stringByAppendingPathComponent:[externalContents objectAtIndex:randomInt(0, numExternalItems - 1)]];
	}
	else
	{
		NSLog(@"no file found");
		filePath = [internalResourcePath stringByAppendingPathComponent:[internalContents objectAtIndex:0]];
	}
	
	NSLog(@"chose path: %@", filePath);
	
	return [filePath cStringUsingEncoding:[NSString defaultCStringEncoding]];
}

@end
