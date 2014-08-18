#!/usr/bin/env python
"""
The build process for this module creates about ~500 MB of temporary files
and I have yet to find a way to cleanup just the temporary ones directly
via node-gyp or gyp.  This cleanup is a bonus, so it always returns success.
"""

import os
import shutil
import sys

def findModule(directory):
	for root, dirs, files in os.walk(directory):
		for file in files:
			if file.endswith(".node"):
				return os.path.join(root, file)

		for name in dirs:
			moduleFilename = findModule(os.path.join(root, name))
			if moduleFilename:
				return moduleFilename

def failedToRemoveBuildDir(function, path, error):
	#print "Warning: failed to remove build dir.  Error:", error
	shutil.rmtree("build_temp", True)
	sys.exit(0)

if __name__ == '__main__':
	moduleFilename = findModule("./build")
	if not moduleFilename:
		sys.exit(0)

	try:
		moduleBasename = os.path.basename(moduleFilename)
		newModuleFilename = os.path.join("build_temp", moduleBasename)

		os.mkdir("build_temp")
		shutil.copy2(moduleFilename, newModuleFilename)
		shutil.rmtree("build", False, failedToRemoveBuildDir)
		shutil.move("build_temp", "build")

		shutil.rmtree("third_party/mdb-conduit/third_party/mongo", True)
	except:
		#print "Warning: failed to cleanup build artifacts.  Error:", sys.exc_info()[1]
		sys.exit(0)
