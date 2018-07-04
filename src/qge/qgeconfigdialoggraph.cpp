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
#include "qgeconfigdialoggraph.h"

#include <pixmapsconfig.h>
#include <qgeconfiggeneralgraphwidget.h>
#include <qgeconfigweightswidget.h>
#include <qgeconfigtypesgraphwidget.h>

#include <qgegraph.h>

QgeConfigDialogGraph::QgeConfigDialogGraph( QgeGraph* qgegraph, QWidget* parent ): 
QgeConfigDialog(parent){
	if( qgegraph )
		this->qgegraph = qgegraph ;
		
	/*-----------------------------------------------------------------------------------------
	* Inicialización y configuracion de los widgets de configuración
	*/
	QPixmap image ;
	
	image.loadFromData( image_general_data, sizeof( image_general_data ), "PNG" );
	QgeConfigGeneralGraphWidget* qgeconfiggeneralgraphwidget = new QgeConfigGeneralGraphWidget( qgegraph, this ) ;
	addConfigWidget( image, "General", qgeconfiggeneralgraphwidget ) ;
	
	image.loadFromData( image_weights_data, sizeof( image_weights_data ), "PNG" );
	QgeConfigWeightsWidget* qgeconfigweightswidget = new QgeConfigWeightsWidget( (QgeItemData*)qgegraph, this ) ;
	addConfigWidget( image, "Weights", qgeconfigweightswidget ) ;
	
	image.loadFromData( image_nodetypes_data, sizeof( image_nodetypes_data ), "PNG" );
	QgeConfigTypesGraphWidget* qgeconfigtypesgraphwidgetnode = new QgeConfigTypesGraphWidget( qgegraph, QgeItem::Rtti_Node, this ) ;
	addConfigWidget( image, "Node Types", qgeconfigtypesgraphwidgetnode ) ;
	
	image.loadFromData( image_edgetypes_data, sizeof( image_edgetypes_data ), "PNG" );
	QgeConfigTypesGraphWidget* qgeconfigtypesgraphwidgetedge = new QgeConfigTypesGraphWidget( qgegraph, QgeItem::Rtti_Edge, this ) ;
	addConfigWidget( image, "Edge Types", qgeconfigtypesgraphwidgetedge ) ;
	//-----------------------------------------------------------------------------------------

}

QgeConfigDialogGraph::~QgeConfigDialogGraph(){
}


