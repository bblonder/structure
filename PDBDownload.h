#include <string>

@interface PDBDownload : NSObject 
{
	NSString *tempPath, *pdbID;
}

- (void) downloadNewPDB;
- (bool) updateList;
//- (NSString*) getPDBName:(std::string)remoteName;

@end
