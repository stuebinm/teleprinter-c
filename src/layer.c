#include "layer.h"



int layer_fetchc (struct layer* l) {
	l->doc->c = l->doc->getc ();
	return l->doc->c;
}
