#include "layer.h"



int layer_fetchc (struct layer* l) {
	l->doc->c = l->doc->fetchc (l->doc);
	return l->doc->c;
}
