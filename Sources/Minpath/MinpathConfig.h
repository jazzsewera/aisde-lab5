#ifndef MINPATH_CONFIG_H
#define MINPATH_CONFIG_H

#include "Exception.h"
#include "MinpathUtils.h"

class MinpathConfig {

	public:
		MinpathConfig(int argc, char** argv);

		char  graphFile[256];
		int   source, destination;
		bool  findAllPaths;
};

#endif // MINPATH_CONFIG_H
