/***************************************************************************
 *   Copyright (C) 2005 by Nestor Aguirre                                  *
 *   nfaguirrec@unal.edu.co                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef QGECONFIGDIALOG_H
#define QGECONFIGDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListBox;
class QPushButton;

class QgeConfigDisplayWidget ;
class QgeConfigPropertiesWidget ;

class QgeItem ;

class QgeConfigDialog : public QDialog{
	Q_OBJECT
	
	public:
		QgeConfigDialog( QWidget* parent = 0, const char* name = 0, bool modal = TRUE, WFlags fl = 0 );
		~QgeConfigDialog();
		
		void updateObject() ;
			
	protected:
		QGridLayout* MyDialog1Layout;
		QHBoxLayout* Layout1;
		QSpacerItem* Horizontal_Spacing2;
	
	protected slots:
		virtual void languageChange();
		void addConfigWidget( QPixmap pixmap, QString label, QWidget* widget ) ;
		void hideAllWidgets() ;
		void changeWidget( const QString& label ) ;
	private:
		void initComponents() ;

	private:
		QListBox* listBox;
		QPushButton* buttonHelp;
		QPushButton* buttonOk;
		QPushButton* buttonCancel;
		
		QMap<QString, QWidget*> mapwidgets ;
};

#endif // QGECONFIGDIALOG_H
