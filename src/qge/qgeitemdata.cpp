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
#include "qgeitemdata.h"

#include <qstring.h>

/**
 * \brief Constructor
 * Crea un objeto del tipo QgeItemData con los valores
 * processed = FALSE Y type = "Default"
 */
QgeItemData::QgeItemData(){
	processed = FALSE ;
	type = QgeItemType() ;
	label = "" ;
	externalId = 0.0 ;
	
	showLabel = TRUE ;
	changetype = FALSE ;
}

/**
 * \brief Destructor
 */
QgeItemData::~QgeItemData(){
}

void QgeItemData::copy( QgeItemData* qgeitemdata ){
	qgeitemdata->isProcessed() ? setProcessed(TRUE) : setProcessed(FALSE) ;
	setExternalId( qgeitemdata->getExternalId() ) ;
	setType( qgeitemdata->getType() ) ;
	setEnableShowLabel( qgeitemdata->isEnableShowLabel() ) ;
	setLabel( qgeitemdata->getLabel() ) ;
	
	weight.clear() ;
	weight = qgeitemdata->getWeights() ;
	changetype = qgeitemdata->isDisableChangeType() ;
}

/**
 * Este metdo será reemplazado por la futura libreria GGTL
 * ya que el saber si el item está o no procesado, solo se
 * ve involucrado en el desarrollo de algoritmos. 
 * \see isProcessed()
 */
void QgeItemData::setProcessed( bool processed ){  this->processed = processed ; }

void QgeItemData::setType( QgeItemType type ){ 
	if( !changetype )
		this->type = type ; 
}

void QgeItemData::setLabel( QString label ){ this->label = label ; }
void QgeItemData::setExternalId( double externalId ){ this->externalId = externalId ; }
void QgeItemData::setDisableChangeType( bool changetype ){ this->changetype = changetype ; }
void QgeItemData::setEnableShowLabel( bool showLabel ){ this->showLabel = showLabel ; }

bool QgeItemData::isProcessed(){ return processed ; }
QgeItemType QgeItemData::getType(){ return type ; }
QString QgeItemData::getLabel(){ return label ; }
double QgeItemData::getExternalId(){ return externalId ; }
bool QgeItemData::isDisableChangeType(){ return changetype ; }
bool QgeItemData::isEnableShowLabel(){ return showLabel ; }

/**
 * Adicionada por conveniencia, pero será reemplazada cuando sea desarrollada 
 * la libreria GGTL ( GNU Graph Template Library ), ya que solo es requerida
 * para calculos sobre grafos ponderados. \n
 * Permite ponderar los nodos, aristas y el grafos con un conjunto de factores
 * identifidados con un QString y un valor numerico de doble presición.
 * @param name Nombre o identificador del factor de peso
 * @param value valor numerico del factor de peso
 */
void QgeItemData::addWeight(QString name, double value){ weight[name] = value ; }

/**
 * Adicionada por conveniencia, pero será reemplazada cuando sea desarrollada 
 * la libreria GGTL ( GNU Graph Template Library ), ya que solo es requerida
 * para calculos sobre grafos ponderados. \n
 * Permite obtener los factores de ponderación de los nodos, aristas o el grafos.
 * \ref addWeight()
 */
QMap<QString, double>& QgeItemData::getWeights(){ return weight ; }
