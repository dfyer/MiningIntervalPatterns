
#include "stdafx.h"   
   
#include"datastruct.h"   
   
Item::Item(int id)   
{   
    this->id=id;   
}   
   
Item::Item()   
{   
    id=-1;   
}   
   
bool Item::operator ==(const Item& another)   
{   
    return this->id==another.id;   
}   
   
ItemSet::ItemSet()   
{   
}   
   
Transaction::Transaction()   
{   
}   
   
ProjectedDB::ProjectedDB(int nTran, int nSet, int nItem)   
{   
    this->nTran=nTran;   
    this->nSet=nSet;   
    this->nItem=nItem;   
}   
   
ProjectedDB::ProjectedDB()   
{   
}   
   
ActiveItem::ActiveItem(int id, bool bJoin)   
{   
    this->id=id;   
    this->bJoin=bJoin;   
}   
   
ActiveItem::ActiveItem()   
{   
}   
   
   
   
BiActiveItem::BiActiveItem(ActiveItem first,ActiveItem second)   
{   
    this->first.bJoin=first.bJoin;   
    this->first.id=first.id;   
    this->second.bJoin=second.bJoin;   
    this->second.id=second.id;   
}   
   
BiActiveItem::BiActiveItem()   
{   
}   
   
void BiActiveItem::operator =(const BiActiveItem& another)   
{   
    first.bJoin=another.first.bJoin;   
    first.id=another.first.id;   
    second.bJoin=another.second.bJoin;   
    second.id=another.second.id;   
}   
   
bool BiActiveItem::operator ==(const BiActiveItem& another)   
{   
    if(first.bJoin==another.first.bJoin && first.id==another.first.id && second.bJoin==another.second.bJoin && second.id==another.second.id)   
        return true;   
    else   
        return false;   
}  
