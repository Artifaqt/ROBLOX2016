/**
 * ogrewidget.h
 * Copyright (c) 2013 NAMEHERE Corp. All Rights Reserved.
 */

#pragma once

// Qt Headers
#include <QString>
#include <QWidget>

// NAMEHERE Headers
#include "util/KeyCode.h"
#include "NAMEHEREView.h"

class NAMEHEREView;

class QOgreWidget : public QWidget
{
   Q_OBJECT
	public:
        QOgreWidget(const QString& name,QWidget* parent = NULL);
	
		void setNAMEHEREView(NAMEHEREView *rbxView);

		void activate();
		void deActivate();

		bool hasApplicationFocus() { return m_hasApplicationFocus; }

        bool luaTextBoxHasFocus() const { return m_luaTextBoxHasFocus; }
        void setLuaTextBoxHasFocus(bool hasFocus) { m_luaTextBoxHasFocus = hasFocus; }

	protected:
		/*override*/bool eventFilter (QObject * watched, QEvent * evt);
		/*override*/bool event(QEvent * evt);
		/*override*/void closeEvent(QCloseEvent *);

		/*override*/void enterEvent(QEvent *);
		/*override*/void leaveEvent(QEvent *);

		/*override*/void focusOutEvent(QFocusEvent* focusEvent);
	
        /*override*/void resizeEvent(QResizeEvent *evt);
	
		/*override*/void mousePressEvent( QMouseEvent *evt );
	    /*override*/void mouseReleaseEvent( QMouseEvent *evt );
	    /*override*/void mouseMoveEvent( QMouseEvent *evt );
	
		/*override*/void keyPressEvent(QKeyEvent *evt);
		/*override*/void keyReleaseEvent(QKeyEvent *evt);

		/*override*/bool focusNextPrevChild(bool next);
	
		/*override*/void wheelEvent(QWheelEvent *evt);
	
		/*override*/void paintEvent(QPaintEvent *evt);

		/*override*/QPaintEngine* paintEngine () const { return NULL; }
	
		//drag-drop related override
		/*override*/void dragEnterEvent(QDragEnterEvent *evt);
		/*override*/void dragMoveEvent(QDragMoveEvent *evt);
		/*override*/void dropEvent(QDropEvent *evt);
		/*override*/void dragLeaveEvent(QDragLeaveEvent *evt);

#ifdef Q_WS_WIN
		virtual bool winEvent(MSG * msg, long * result);
#endif
		
    private:
		typedef QWidget Super;

		bool isValidDrag(QDragEnterEvent *evt);

		void handleKeyEvent(QKeyEvent * evt,
                            RBX::InputObject::UserInputType eventType,
                            RBX::InputObject::UserInputState eventState,
							bool processed = false);
	
		NAMEHEREView	*m_pNAMEHEREView;
		int			m_bIgnoreEnterEvent;
		bool		m_bIgnoreLeaveEvent;
		bool		m_bUpdateInProgress;
		bool		m_bMouseCommandInvoked;
		bool		m_hasApplicationFocus;
        bool        m_bNAMEHEREViewInitialized;
        bool        m_luaTextBoxHasFocus;

		QPoint lastMovePoint;
		RBX::ModCode lastMoveModCode;
 };
