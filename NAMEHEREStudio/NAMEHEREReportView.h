/**
 * NAMEHEREReportView.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QTreeWidget>
#include <QStyledItemDelegate>

class NAMEHERECategoryItemDelegate : public QStyledItemDelegate
{
public:
	NAMEHERECategoryItemDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
	void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const;
};

class NAMEHERECategoryItem: public QTreeWidgetItem
{
public:
	NAMEHERECategoryItem();
};

class NAMEHEREReportView : public QTreeWidget
{
public:
	NAMEHEREReportView(); 
	virtual ~NAMEHEREReportView();

	void addCategoryItem(NAMEHERECategoryItem* pItem);
	bool isCategoryItem(const QModelIndex &index);
	NAMEHERECategoryItem* findCategoryItem(const QString& category);
	bool addToCategory(const QString& category, QTreeWidgetItem* pItem);
		
private:
	/*override*/void drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	/*override*/void drawBranches(QPainter * painter, const QRect & rect, const QModelIndex & index ) const;
};


