#include "genericmindmapitem.h"

GenericMindMapItem::GenericMindMapItem()
    : m_stringManager(NULL)
{

}

void GenericMindMapItem::setStringManager(StringManager* stringManager)

{
    m_stringManager = stringManager;
}
