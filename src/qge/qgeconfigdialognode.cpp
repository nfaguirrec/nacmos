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
#include "qgeconfigdialognode.h"

#include <qcombobox.h>
#include <qpixmap.h>
#include <qgenode.h>

#include <pixmapsconfig.h>
#include <qgeconfigdisplaywidget.h>
#include <qgeconfigpropertieswidget.h>
#include <qgeconfigweightswidget.h>

QgeConfigDialogNode::QgeConfigDialogNode( QgeNode* qgenode, QStringList types, QWidget* parent )
:QgeConfigDialog( parent ){
	/*-----------------------------------------------------------------------------------------
	* Inicialización y configuracion de los widgets de configuración
	*/
	QPixmap image ;
	
	image.loadFromData( image_general_data, sizeof( image_general_data ), "PNG" );
	qgeconfigpropertieswidget = new QgeConfigPropertiesWidget( (QgeItem*)qgenode, this ) ;
	qgeconfigpropertieswidget->setTypes( types ) ;
	addConfigWidget( image, "General", qgeconfigpropertieswidget ) ;

	image.loadFromData( image_display_data, sizeof( image_display_data ), "PNG" );
	qgeconfigdisplaywidget = new QgeConfigDisplayWidget( (QgeItem*)qgenode, this ) ;
	addConfigWidget( image, "Display", qgeconfigdisplaywidget ) ;
	
	image.loadFromData( image_weights_data, sizeof( image_weights_data ), "PNG" );
	qgeconfigweightswidget = new QgeConfigWeightsWidget( (QgeItemData*)(qgenode), this ) ;
	addConfigWidget( image, "Weights", qgeconfigweightswidget ) ;
	//-----------------------------------------------------------------------------------------
}

QgeConfigDialogNode::~QgeConfigDialogNode(){
}


