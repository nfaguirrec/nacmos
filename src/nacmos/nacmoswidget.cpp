/***************************************************************************
 *   Copyright (C) 2007 by Nestor Aguirre   *
 *   nfaguirrec@unal.edu.co   *
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
#include "nacmoswidget.h"

#include <qlabel.h>
#include <qmessagebox.h>
#include <qcanvas.h>
#include <qpushbutton.h>
#include <qtextedit.h>

#include <qgewidget.h>

#include <swapmanager.h>
#include <outputstream.h>

nacmosWidget::nacmosWidget(QWidget* parent, const char* name, WFlags fl)
        : nacmosWidgetBase(parent,name,fl)
{
	weightingnodeswizard = new WeightingNodesWizard() ;
	
// 	connect( weightingnodeswizard, SIGNAL(finished()), this, SLOT(activateButtons()) ) ;
	
	clusteringwithps = new ClusteringWithPS() ;
	clusteringwithsom = new ClusteringWithSOM() ;
	
// 	hadBuiltDatabases = FALSE ;
	
	secondaryDB = new SwapFileViewer() ;
	secondaryDB->setSwapFileId( SwapManager::SECONDARY ) ;
	
	principalDB = new SwapFileViewer() ;
	principalDB->setSwapFileId( SwapManager::PRINCIPAL ) ;
	
	graphseditor = new GraphsEditor() ;
	
	OutputStream::Instance( OutputStream::LOGS, textEditLog ) << "Welcome to NAcMoS" << endl ;
	
	connect( SwapManager::Instance( SwapManager::PRINCIPAL ), SIGNAL(hasChanged(bool)), this, SLOT(updateDataBasesViewers(bool)) ) ;
	connect( SwapManager::Instance( SwapManager::SECONDARY ), SIGNAL(hasChanged(bool)), this, SLOT(updateDataBasesViewers(bool)) ) ;
}

nacmosWidget::~nacmosWidget()
{
}

/*$SPECIALIZATION$*/
void nacmosWidget::weightingGraphs()
{
	weightingnodeswizard->show() ;
}

void nacmosWidget::clusteringWithSOM()
{
	if( SwapManager::Instance( SwapManager::SECONDARY )->getSize() == 0 ){
		QMessageBox::information( this, tr("Error !!!!"), tr("This option is only available when you have graphs\n" 			"in secondary database\n\n"), QMessageBox::Ok) ;
		return ;
	}

	clusteringwithsom->show() ;
	
	OutputStream::Instance( OutputStream::LOGS, textEditLog ) << "Loading module: Clustering with SOM" << endl ;

}

void nacmosWidget::clusteringWithPS()
{
	if( SwapManager::Instance( SwapManager::PRINCIPAL )->getSize() == 0 ){
		QMessageBox::information( this, tr("Error !!!!"), tr("This option is only available when you have graphs\n" 			"in principal database\n\n"), QMessageBox::Ok) ;
		return ;
	}

	clusteringwithps->show() ;
	
	OutputStream::Instance( OutputStream::LOGS, textEditLog ) << "Loading module: Clustering with PS" << endl ;
}

void nacmosWidget::updateDataBasesViewers( bool firstTime )
{
	secondaryDB->update( firstTime ) ;
	principalDB->update( firstTime ) ;
}

// void nacmosWidget::/*activateButtons*/()
// {
// 	hadBuiltDatabases = TRUE ;
// }

void nacmosWidget::viewSecondaryDatabase()
{
	secondaryDB->show() ;
}


void nacmosWidget::viewPrincipalDatabase()
{
	principalDB->show() ;
}

void nacmosWidget::graphsEditor()
{
	graphseditor->show() ;
}



