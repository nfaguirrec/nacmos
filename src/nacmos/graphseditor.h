/***************************************************************************
 *   Copyright (C) 2007 by Universidad Nacional de Colombia   *
 *   http://www.unal.edu.co   *
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
#ifndef GRAPHSEDITOR_H
#define GRAPHSEDITOR_H

#include <qstring.h>

#include <graphseditorbase.h>

class GraphsEditor : public GraphsEditorBase {
	
	public:
		GraphsEditor( QWidget* parent = 0, const char* name = 0 );
		~GraphsEditor();
		
	protected:
		void callViennaRNAPackage( QString fastaFileName ) ;

	public slots:
		virtual void secondaryStructureViennaPackage();
		virtual void verifingFragmentation();
		virtual void fileSaveAsFasta();
		virtual void fileSaveFasta();
		virtual void fileOpenFasta();
		virtual void reload();
		virtual void newGraph();
		virtual void print();
		virtual void fileSaveAs();
		virtual void fileSave();
		virtual void fileOpen();
		virtual void getDijsktraPath();
		virtual void removeEdge();
		virtual void removeNode();
		virtual void hideEdge();
		virtual void hideNode();
		virtual void moveNode();
		virtual void insertEdge();
		virtual void insertNode();
		virtual void moveGraph();
		
	private:
		QString fileNameFasta ;
};

// #include <qmainwindow.h>
// 
// class GraphsEditor : public QMainWindow {
// 
// 	public:
// 		GraphsEditor(QWidget *parent=0, const char *name=0) ;
// 		~GraphsEditor() ;
// 		
// 	private:
// 		QgeWidget* qgewidget ;
// };


#endif
