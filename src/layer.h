#ifndef LAYER_H
#define LAYER_H

#include "document.h"

struct layer {
	struct document* doc;
	
};

int layer_fetchc (struct layer* l);




#endif
