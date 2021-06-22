#include <zlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <iostream>

#import "PDBDownload.h"
#include "RandomUtil.h"
#include "FileUtility.h"
#include "StringHelper.h"

@implementation PDBDownload

/*- (NSString*) getPDBName:(std::string)remoteName
{
	return [[[NSString stringWithCString:remoteName.substr(3,4).c_str() encoding:[NSString defaultCStringEncoding]] uppercaseString] retain];
}*/

- (bool) updateList
{
	NSString* theFile = [NSString stringWithContentsOfURL:[NSURL URLWithString:@"ftp://ftp.wwpdb.org/pub/pdb/derived_data/pdb_entry_type.txt"] encoding:[NSString defaultCStringEncoding] error:nil];
	if (theFile)
	{
		NSLog(@"Downloaded successfully");
		NSString* localDir = [[NSBundle bundleForClass:NSClassFromString(@"StructureOpenGLView")] resourcePath];
		bool exists = [[NSFileManager defaultManager] fileExistsAtPath:localDir];
		if (!exists)
		{
			[[NSFileManager defaultManager] createDirectoryAtPath:localDir withIntermediateDirectories:YES attributes:NULL error:NULL];
			NSLog(@"had to create the resource directory - bad!");
		}
		NSString *localPath = [localDir stringByAppendingPathComponent:@"pdb_entry_type.txt"];
		
		bool result = [theFile writeToFile:localPath atomically:YES encoding:[NSString defaultCStringEncoding] error:nil];
		if (result)
		{
			NSLog(@"Wrote file to %@", localPath);
			return true;
		}
		return false;
	}
	return false;
}

- (void) downloadNewPDB
{
/*	std::string basePath = "ftp://ftp.wwpdb.org/pub/pdb/data/structures/divided/pdb/";
	std::vector<std::string> baseContents, subContents;
	
	if (printFTPDirectory(basePath, &baseContents))
	{	
		int numBaseContents = baseContents.size();
		if (numBaseContents >= 1)
		{
			std::string subPath = basePath + baseContents[randomInt(0, numBaseContents - 1)] + "/";
		
			if (printFTPDirectory(subPath, &subContents))
			{
				int numSubContents = subContents.size();
				if (numSubContents >= 1)
				{
					std::string fileName = subContents[randomInt(0, numSubContents - 1)];
					std::string finalPath = subPath + fileName;
					
					pdbID = [self getPDBName:fileName];
					
					tempPath = [NSString stringWithCString:tmpnam(NULL) encoding:[NSString defaultCStringEncoding]];
					
					NSURLRequest *request = [NSURLRequest 
						requestWithURL:[NSURL URLWithString:[NSString stringWithCString:finalPath.c_str() encoding:[NSString defaultCStringEncoding]]]
						cachePolicy:NSURLRequestUseProtocolCachePolicy
						timeoutInterval:30.0];
						
					NSURLDownload *download = [[NSURLDownload alloc] initWithRequest:request delegate:self];
					[download setDestination:tempPath allowOverwrite:YES];
					
					if (!download)
					{
						NSLog(@"something bad happened to the download");
					}
				}
				else
				{
					NSLog(@"problem with subcontents");
				}
			}
		}
		else
		{
			NSLog(@"problem with basecontents");
		}
	}
 */
	

	
	NSString *fullResourcePath = [[[NSBundle bundleForClass:NSClassFromString(@"PDBDownload")] resourcePath] stringByAppendingPathComponent:@"pdb_entry_type.txt"];
	//	NSLog(@"reading list file at %@", fullResourcePath);
	
	std::vector<std::string> pdbNames;
	std::string tempString;
	std::ifstream ifs;
	std::string fullResourcePathCString = [fullResourcePath cStringUsingEncoding:[NSString defaultCStringEncoding]];
	//NSLog(@"full path is %s", fullResourcePathCString.c_str());
	
	ifs.open(fullResourcePathCString.c_str(), std::ifstream::in); // need to error-check this read
	
	if (ifs)
	{
		while(getline(ifs,tempString))
		{
			std::string tempCode = tempString.substr(0, 4);
			pdbNames.push_back(tempCode);
			//NSLog(@"pdb found: %s", tempCode.c_str());
		}
		ifs.close();
		
		//NSLog(@"read %d lines", pdbNames.size());
		
		pdbID = [NSString stringWithFormat:@"%s", pdbNames.at(randomInt(0, pdbNames.size() - 1)).c_str()];

		//NSLog(@"chose %@", pdbID);
		
		NSString *finalPath = [NSString stringWithFormat:@"http://www.rcsb.org/pdb/files/%@.pdb", pdbID];
		
		NSLog(@"Downloading from %@", finalPath);
		
		NSString* thePDBFile = [NSString stringWithContentsOfURL:[NSURL URLWithString:finalPath] encoding:[NSString defaultCStringEncoding] error:nil];
		if (thePDBFile)
		{
			NSString* localDir = [[NSBundle bundleForClass:NSClassFromString(@"StructureOpenGLView")] resourcePath];
			bool exists = [[NSFileManager defaultManager] fileExistsAtPath:localDir];
			if (!exists)
			{
				[[NSFileManager defaultManager] createDirectoryAtPath:localDir withIntermediateDirectories:YES attributes:NULL error:NULL];
				NSLog(@"had to create the resource directory - bad!");
			}
			NSString *localPath = [[localDir stringByAppendingPathComponent:pdbID] stringByAppendingString:@".pdb"];
			bool result = [thePDBFile writeToFile:localPath atomically:YES encoding:[NSString defaultCStringEncoding] error:nil];
			if (result)
			{
				NSLog(@"Wrote file to %@", localPath);
				NSArray *contents = [FileUtility filesWithExtension:@"PDB" withArray:[[NSFileManager defaultManager] contentsOfDirectoryAtPath:localDir error:nil]];
				if (contents != nil)
				{
					int numFilesToRemove = [contents count] - [[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] integerForKey:@"CacheSize"]; // cache size (fixed)
					for (int i=0; i<numFilesToRemove; i++)
					{
						int numContentsCount = [contents count];
						if (numContentsCount >= 1)
						{
							NSString *fullPath = [localDir stringByAppendingPathComponent:[contents objectAtIndex:randomInt(0, numContentsCount - 1)]];
							[[NSFileManager defaultManager] removeItemAtPath:fullPath error:nil];
							NSLog(@"removing %@", fullPath);
						}
						else
						{
							NSLog(@"numContentsCount bad");
						}
					}
				}
				else
				{
					NSLog(@"uh oh - directory didn't exist!");
				}				
			}
			else
			{
				NSLog(@"failed to write");
			}
			
			
		}
		else
		{
			NSLog(@"failed to download");
		}
		
		/*NSURLRequest *request =[NSURLRequest requestWithURL:[NSURL URLWithString:finalPath]];
		NSURLDownload *download = [[NSURLDownload alloc] initWithRequest:request delegate:self];
		[download setDestination:[NSString stringWithFormat:@"%s", tmpnam(NULL)] allowOverwrite:YES];
		
		if (!download)
		{
			NSLog(@"something bad happened to the download");
		}*/
	}
	else
	{
		NSLog(@"failed to read pdb list file");
	}
}

/*- (void)download:(NSURLDownload *)download didFailWithError:(NSError *)error
{
    [download release];

    NSLog(@"Download failed! Error - %@", [error localizedDescription]);
}

- (void)downloadDidFinish:(NSURLDownload *)download
{
    [download release];
	
	NSString* localDir = [[NSBundle bundleForClass:NSClassFromString(@"StructureOpenGLView")] resourcePath];
	bool exists = [[NSFileManager defaultManager] fileExistsAtPath:localDir];
	if (!exists)
	{
		[[NSFileManager defaultManager] createDirectoryAtPath:localDir withIntermediateDirectories:YES attributes:NULL error:NULL];
		NSLog(@"had to create the resource directory - bad!");
	}
	
	NSString *localPath = [[localDir stringByAppendingPathComponent:pdbID] stringByAppendingString:@".pdb"];
	
	[[NSFileManager defaultManager] moveItemAtPath:tempPath toPath:localPath error:NULL];
	
	
	
	NSArray *contents = [FileUtility filesWithExtension:@"PDB" withArray:[[NSFileManager defaultManager] contentsOfDirectoryAtPath:localDir error:nil]];
	if (contents != nil)
	{
		int numFilesToRemove = [contents count] - [[ScreenSaverDefaults defaultsForModuleWithName:@"Structure"] integerForKey:@"CacheSize"]; // cache size (fixed)
		for (int i=0; i<numFilesToRemove; i++)
		{
			int numContentsCount = [contents count];
			if (numContentsCount >= 1)
			{
				NSString *fullPath = [localDir stringByAppendingPathComponent:[contents objectAtIndex:randomInt(0, numContentsCount - 1)]];
				[[NSFileManager defaultManager] removeItemAtPath:fullPath error:nil];
				NSLog(@"removing %@", fullPath);
			}
			else
			{
				NSLog(@"numContentsCount bad");
			}
		}
	}
	else
	{
		NSLog(@"uh oh - directory didn't exist!");
	}
}*/
@end
