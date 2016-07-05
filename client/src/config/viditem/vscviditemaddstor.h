#ifndef __VSC_VID_ITEM_ADDSTOR_H__
#define __VSC_VID_ITEM_ADDSTOR_H__

#include "common/vscviditeminf.h"

class VSCVidItemAddStor : public QObject,public VSCVidItemInf
{
Q_OBJECT
public:
    VSCVidItemAddStor(ClientFactory &pFactory, QTreeWidgetItem *parent);
    ~VSCVidItemAddStor();
public:

};

#endif /* __VSC_VID_ITEM_ADDSTOR_H__ */