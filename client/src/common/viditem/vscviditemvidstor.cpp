#include "common/viditem/vscviditemvidstor.h"

VSCVidItemVidStor::VSCVidItemVidStor(VidStor cStor, ClientFactory &pFactory, QTreeWidgetItem *parent)
: m_cStor(cStor), VSCVidItemInf(pFactory, parent)
{
	/* if came here, all the item is ready, just start the cStor client */
}
VSCVidItemVidStor::~VSCVidItemVidStor()
{
	
}