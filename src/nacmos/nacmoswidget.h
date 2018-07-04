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
#ifndef _NACMOSWIDGET_H_
#define _NACMOSWIDGET_H_

#include "nacmoswidgetbase.h"

#include <weightingnodeswizard.h>
#include <clusteringwithps.h>
#include <clusteringwithsom.h>
#include <swapfileviewer.h>
#include <graphseditor.h>

class nacmosWidget : public nacmosWidgetBase
{
	Q_OBJECT
	
	public:
		nacmosWidget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
		~nacmosWidget();
		/*$PUBLIC_FUNCTIONS$*/
	
	public slots:
		/*$PUBLIC_SLOTS$*/
		virtual void weightingGraphs();
		virtual void clusteringWithSOM();
		virtual void clusteringWithPS();
		virtual void updateDataBasesViewers( bool firstTime );
// 		virtual void activateButtons() ;
		virtual void viewSecondaryDatabase();
		virtual void viewPrincipalDatabase();
    		virtual void graphsEditor();
		
	protected:
		/*$PROTECTED_FUNCTIONS$*/
	
	protected slots:
		/*$PROTECTED_SLOTS$*/
	
	private:
		WeightingNodesWizard* weightingnodeswizard ;
		ClusteringWithPS* clusteringwithps ;
		ClusteringWithSOM* clusteringwithsom ;
// 		bool hadBuiltDatabases ;
		
		SwapFileViewer* secondaryDB ;
		SwapFileViewer* principalDB ;
		
		GraphsEditor* graphseditor ;
};

#endif

