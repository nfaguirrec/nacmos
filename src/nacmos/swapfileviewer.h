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
#ifndef SWAPFILEVIEWER_H
#define SWAPFILEVIEWER_H

#include "swapfileviewerbase.h"

/**
	@author Nestor Aguirre <nfaguirrec@unal.edu.co>
*/
class SwapFileViewer : public SwapFileViewerBase
{
	public:
		SwapFileViewer( QWidget* parent = 0, const char* name = 0 );
		~SwapFileViewer();
		
		void setSwapFileId( uint swapFile ) ;
		
	public slots:
		virtual void viewGraph( int id );
		virtual void removeCurrentGraph();
		virtual void clear() ;
		virtual void update( bool firstTime ) ;
    		virtual void load();
		virtual void save();
		
	private:
		int swapFile ;
};

#endif
