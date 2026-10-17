/**
 * NAMEHEREReportView.cpp
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#include "stdafx.h"
#include "NAMEHEREReportView.h"

// Qt Headers
#include <QPainter>

#ifdef Q_WS_MAC
    #define CATEGORY_ITEM_PADDING	7
    #define CATEGORY_ITEM_INDENT	24
#else
    #define CATEGORY_ITEM_PADDING	3
    #define CATEGORY_ITEM_INDENT    20
#endif

NAMEHERECategoryItem::NAMEHERECategoryItem()
{
	setFlags(Qt::ItemIsEnabled);

	QFont boldFont(font(0));
	boldFont.setBold(true);
	boldFont.setPointSize(9);
	setFont(0, boldFont);

	setTextAlignment(0, Qt::AlignJustify | Qt::AlignVCenter);
}

void NAMEHERECategoryItemDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
	QStyledItemDelegate::initStyleOption(option, index);
	NAMEHEREReportView *pTree = dynamic_cast<NAMEHEREReportView*>(parent());
	if(pTree->isCategoryItem(index)) 
	{
		QRect rect = option->rect;
		QRect newRect(rect.left()+CATEGORY_ITEM_INDENT, rect.top(), rect.width(), rect.height()+CATEGORY_ITEM_PADDING);
		option->rect = newRect;
	}
}

QSize NAMEHERECategoryItemDelegate::sizeHint(const QStyleOptionViewItem &option,
											  const QModelIndex &index) const
{
	QSize sz = QStyledItemDelegate::sizeHint(option, index);
	NAMEHEREReportView *pTree = dynamic_cast<NAMEHEREReportView*>(parent());
	if(pTree->isCategoryItem(index)) 
	{		
		sz.setWidth(sz.width() - CATEGORY_ITEM_INDENT);
		sz.setHeight(sz.height() + CATEGORY_ITEM_PADDING);
	}
	return sz;
}

NAMEHEREReportView::NAMEHEREReportView()
{
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setAlternatingRowColors(true);
	setIndentation(0);

	NAMEHERECategoryItemDelegate *pItemDelegate = new NAMEHERECategoryItemDelegate(this);
	setItemDelegate(pItemDelegate);
}

NAMEHEREReportView::~NAMEHEREReportView()
{}

void NAMEHEREReportView::addCategoryItem(NAMEHERECategoryItem* pItem)
{
	if(!pItem)
		return;

	addTopLevelItem(pItem);
	setFirstItemColumnSpanned(pItem, true);
	
	QSize sz = pItem->sizeHint(0);
	sz.scale(sz.width(), sz.height()*2, Qt::KeepAspectRatio);
	pItem->setSizeHint(0, sz);
}

NAMEHERECategoryItem* NAMEHEREReportView::findCategoryItem(const QString& category)
{
	NAMEHERECategoryItem* pCategoryItem = NULL;
	QList<QTreeWidgetItem*> items = findItems(category, Qt::MatchExactly);
	if(items.count())
		pCategoryItem = dynamic_cast<NAMEHERECategoryItem*>(items[0]);
	
	return pCategoryItem;
}

bool NAMEHEREReportView::addToCategory(const QString& category, QTreeWidgetItem* pItem)
{
	if(!pItem)
		return false;

	NAMEHERECategoryItem* pCategory = findCategoryItem(category);
	if(pCategory)
	{
		pCategory->addChild(pItem);
		return true;
	}

	return false;
}

void NAMEHEREReportView::drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItemV3 opt = option;
	
	NAMEHERECategoryItem* pCategoryItem = dynamic_cast<NAMEHERECategoryItem*>(itemFromIndex(index));
	if(pCategoryItem)
	{
		const QColor c = option.palette.color(QPalette::Dark);
        painter->fillRect(option.rect, c);
        opt.palette.setColor(QPalette::AlternateBase, c);
	}

	QTreeWidget::drawRow(painter, opt, index);
}

void NAMEHEREReportView::drawBranches(QPainter * painter, const QRect & rect, const QModelIndex & index ) const
{
	NAMEHERECategoryItem* pCategoryItem = dynamic_cast<NAMEHERECategoryItem*>(itemFromIndex(index));
	if(pCategoryItem)
	{
		QRect primitive(rect.left(), rect.top(), indentation()+CATEGORY_ITEM_INDENT, rect.height()+CATEGORY_ITEM_PADDING);

		QStyleOptionViewItemV4 opt = viewOptions();
		QStyle::State extraFlags = QStyle::State_None;
		if (isEnabled())
			extraFlags |= QStyle::State_Enabled;
		if (window()->isActiveWindow())
			extraFlags |= QStyle::State_Active;
		QPoint oldBO = painter->brushOrigin();
		if (verticalScrollMode() == QAbstractItemView::ScrollPerPixel)
			painter->setBrushOrigin(QPoint(0, verticalOffset()));

		if(alternatingRowColors())
		{
			opt.features |= QStyleOptionViewItemV4::Alternate;
		}

		if(selectionModel()->isSelected(index))
			extraFlags |= QStyle::State_Selected;

        opt.rect = primitive;
		opt.state = QStyle::State_Item | extraFlags | QStyle::State_Children
					| (pCategoryItem->isExpanded() ? QStyle::State_Open : QStyle::State_None);

		style()->drawPrimitive(QStyle::PE_IndicatorBranch, &opt, painter, this);
		
		painter->setBrushOrigin(oldBO);
	}
	else
	{
		QTreeWidget::drawBranches(painter, rect, index );
	}
}

bool NAMEHEREReportView::isCategoryItem(const QModelIndex &index)
{
	NAMEHERECategoryItem* pCategoryItem = dynamic_cast<NAMEHERECategoryItem*>(itemFromIndex(index));
	return (pCategoryItem != NULL);
}

