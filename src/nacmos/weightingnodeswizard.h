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
#ifndef WEIGHTINGNODESWIZARD_H
#define WEIGHTINGNODESWIZARD_H

#include "weightingnodeswizardbase.h"

#include <qmap.h>
#include <qstring.h>

#include <qgegraph.h>
#include <addadrawngraph.h>
#include <fragmenter.h>
#include <swapfileviewer.h>

/**
@author Nestor Aguirre
*/
class WeightingNodesWizard : public WeightingNodesWizardBase
{
	Q_OBJECT
	
	public:
		WeightingNodesWizard( QWidget* parent = 0, const char* name = 0 );
		~WeightingNodesWizard();
		
	protected slots:
		/**************************************************
		* Todas las paginas
		*/
		virtual void currentChanged( const QString& pageName ) ;
		
		/**************************************************
		* Primera página
		*/
		virtual void run() ;
		virtual void viewPrincipalGraphs() ;
		virtual void viewTemplateGraphs() ;
		virtual void drawGraphs() ;
		virtual void loadGraphs();
		virtual void loadLabelGraphs();
		virtual void viewGraphsLoad();
		virtual void loadGraphsAndSequences();
		virtual void drawGraphsAndSequences();
		
		/**************************************************
		* Segunda página
		*/
		virtual void viewSubgraph( int row ) ;
		virtual void loadNames();
		virtual void getNameGQT();
		
		/**************************************************
		* Tercera página
		*/
		virtual void loadProperties() ;
		virtual void removeProperty() ;
		virtual void addProperty() ;
		virtual void accept() ;
		virtual void viewGraphsViennaRNA();
		virtual void loadFastaViennaRNA();
		
	signals:
		void finished() ;
		
	private:
		/**************************************************
		* Segunda página
		*/
		void updateTableMotifNames() ;
		static QString getNameGQT( QgeGraph* qgegraph ) ;
		static QgeNode* getNode( QgeNode* qgenode, QgeGraph* qgegraph, QString itemtypeName, int direction ) ;
		
		/**************************************************
		* Tercera página
		*/
		void updateTableProperties() ;
		
	private:
		/**************************************************
		* Segunda página
		*/
		Fragmenter* fragmenter ;
		QMap<double, QString> translationMap ;
		QString current ;
		
		/**************************************************
		* Tercera página
		*/
		QMap< QString, QValueList<double> > weightsMap ;
		QStringList nameColumns ;
		
		QgeGraph* tmpGraph ;
		
		SwapFileViewer* viewPrincipalSwapFile ;
		SwapFileViewer* viewTemplatesSwapFile ;
};

#endif
