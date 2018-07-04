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
#include "qgeconfigdialog.h"

#include <qvariant.h>
#include <qlistbox.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/**
 *  Constructs a QgeGeneralConfigDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
QgeConfigDialog::QgeConfigDialog( QWidget* parent, const char* name, bool modal, WFlags fl ):
QDialog( parent, name, modal, fl ){
	initComponents() ;
}

/**
 *  Destroys the object and frees any allocated resources
 */
QgeConfigDialog::~QgeConfigDialog(){
	// no need to delete child widgets, Qt does it all for us
}

void QgeConfigDialog::updateObject(){
	QValueList<QString> listlabels = mapwidgets.keys() ;
	for( QValueList<QString>::iterator it = listlabels.begin(); it != listlabels.end(); it++ ){
		mapwidgets[*it]->close( TRUE ) ;
	}
}

void QgeConfigDialog::initComponents(){
	
	setSizeGripEnabled( TRUE );
	MyDialog1Layout = new QGridLayout( this, 1, 1, 11, 6, "MyDialog1Layout"); 
	
	/*-----------------------------------------------------------------------------------------
	* Inicialización y configuracion del listbox
	*/
	listBox = new QListBox( this, "listBox" );
	listBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)7, 0, 0, listBox->sizePolicy().hasHeightForWidth() ) );
	listBox->setMinimumSize( QSize( 150, 0 ) );
	
	MyDialog1Layout->addWidget( listBox, 0, 0 ) ;
	//-----------------------------------------------------------------------------------------
		
	/*-----------------------------------------------------------------------------------------
	* Inicialización y configuracion de los botones básicos del dialogo
	*/
	Layout1 = new QHBoxLayout( 0, 0, 6, "Layout1"); 
	
	buttonHelp = new QPushButton( this, "buttonHelp" );
	buttonHelp->setAutoDefault( TRUE );
	Layout1->addWidget( buttonHelp );
	Horizontal_Spacing2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1->addItem( Horizontal_Spacing2 );
	
	buttonOk = new QPushButton( this, "buttonOk" );
	buttonOk->setAutoDefault( TRUE );
	buttonOk->setDefault( TRUE );
	Layout1->addWidget( buttonOk );
	
	buttonCancel = new QPushButton( this, "buttonCancel" );
	buttonCancel->setAutoDefault( TRUE );
	Layout1->addWidget( buttonCancel );
	//-----------------------------------------------------------------------------------------
	
	MyDialog1Layout->addMultiCellLayout( Layout1, 1, 1, 0, 1 );
	languageChange();
	resize( QSize(600, 400).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );
	
	// signals and slots connections
	connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( listBox, SIGNAL( highlighted( const QString& ) ), this, SLOT( changeWidget( const QString& ) ) ) ;
}

/**
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QgeConfigDialog::languageChange(){
	setCaption( tr( "Qge General Config Dialog" ) );
	buttonHelp->setText( tr( "&Help" ) );
	buttonHelp->setAccel( QKeySequence( tr( "F1" ) ) );
	buttonOk->setText( tr( "&OK" ) );
	buttonOk->setAccel( QKeySequence( QString::null ) );
	buttonCancel->setText( tr( "&Cancel" ) );
	buttonCancel->setAccel( QKeySequence( QString::null ) );
}

void QgeConfigDialog::addConfigWidget( QPixmap pixmap, QString label, QWidget* widget ){
	mapwidgets[ label ] = widget ;
	MyDialog1Layout->addWidget( widget, 0, 1 ) ;
	listBox->insertItem( pixmap, label );
}

void QgeConfigDialog::hideAllWidgets(){
	QValueList<QString> listlabels = mapwidgets.keys() ;
	for( QValueList<QString>::iterator it = listlabels.begin(); it != listlabels.end(); it++ ){
		mapwidgets[*it]->hide() ;
	}
}

void QgeConfigDialog::changeWidget( const QString& label ){
	hideAllWidgets() ;
	mapwidgets[label]->show() ;
}

