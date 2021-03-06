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
#include "dendrogramwidget.h"

#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>

#include <qwt/qwt_counter.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_math.h>
#include <qwt/qwt_plot.h>

#include <iostream>
using namespace std ;

#include <dendrogram.h>

static const unsigned char image0_data[] = {
	0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7a, 0x7a, 0xf4, 0x00, 0x00, 0x06,
	0x8f, 0x49, 0x44, 0x41, 0x54, 0x58, 0x85, 0xad, 0x96, 0x6d, 0x8c, 0x15,
	0x57, 0x19, 0xc7, 0x7f, 0x73, 0x66, 0xe6, 0xcc, 0xcc, 0x7d, 0x59, 0x2e,
	0xb8, 0x85, 0x02, 0x45, 0x16, 0x69, 0x01, 0x4d, 0x5b, 0x57, 0x8a, 0x6b,
	0x90, 0x36, 0xd9, 0x6d, 0xb5, 0x8d, 0xad, 0x04, 0x30, 0x85, 0xda, 0x34,
	0xca, 0x1a, 0x3f, 0xa8, 0x1f, 0x4c, 0x21, 0x34, 0xd5, 0xc4, 0xa6, 0x10,
	0xc4, 0xa4, 0x90, 0x20, 0x12, 0x3e, 0xa8, 0xd1, 0xca, 0x2e, 0x25, 0x25,
	0x2b, 0x6d, 0x61, 0xdb, 0x60, 0x81, 0x26, 0x85, 0x2a, 0xad, 0x11, 0x14,
	0x36, 0x1a, 0xde, 0xa4, 0xd2, 0x85, 0xb2, 0xbc, 0x2e, 0xec, 0x65, 0x59,
	0xee, 0xbd, 0x73, 0xee, 0xcc, 0x39, 0x7e, 0x18, 0xf6, 0xee, 0x96, 0x17,
	0xbd, 0x57, 0x79, 0x92, 0x93, 0x7b, 0x73, 0x72, 0xe6, 0x79, 0x7e, 0xcf,
	0xff, 0x39, 0xcf, 0x33, 0x63, 0x71, 0x0b, 0xdb, 0xb1, 0xe3, 0xfd, 0x56,
	0xdb, 0x36, 0x73, 0x84, 0xa0, 0x39, 0x08, 0xbc, 0x9c, 0xef, 0x4b, 0xa2,
	0x28, 0x62, 0x60, 0xa0, 0x80, 0xd6, 0xec, 0xd6, 0x9a, 0x4e, 0xad, 0x45,
	0xdb, 0x63, 0x8f, 0xcd, 0xca, 0xdf, 0xca, 0x47, 0x35, 0x66, 0x5d, 0xbf,
	0xd1, 0xd9, 0xf9, 0x5e, 0xb3, 0x94, 0x7a, 0xbd, 0x26, 0x6a, 0x88, 0x10,
	0x78, 0xa9, 0x34, 0xc6, 0x18, 0x84, 0x10, 0x08, 0x21, 0xd0, 0x5a, 0xe3,
	0x58, 0x10, 0x97, 0x43, 0x88, 0x54, 0x3e, 0x8a, 0xc4, 0xda, 0xc7, 0x1f,
	0x7f, 0x78, 0xd9, 0x6d, 0x01, 0x78, 0xfd, 0xf5, 0x77, 0x96, 0xb9, 0x32,
	0x5e, 0x5a, 0xb6, 0x6c, 0x52, 0x99, 0x0c, 0x99, 0x4c, 0xaa, 0x12, 0x58,
	0x58, 0x20, 0x84, 0xc0, 0x12, 0x16, 0x16, 0x16, 0xaa, 0x1c, 0x51, 0x2c,
	0x14, 0x09, 0x07, 0x2e, 0x13, 0x85, 0x6a, 0x77, 0x18, 0xda, 0xf3, 0x9e,
	0x7c, 0xf2, 0xd1, 0x9a, 0xd5, 0xa8, 0x00, 0x6c, 0xda, 0xf4, 0x87, 0x65,
	0xae, 0x17, 0x2d, 0xb5, 0xfc, 0x2c, 0xf5, 0xf5, 0x39, 0x1c, 0xc7, 0xc1,
	0x75, 0x6c, 0x6c, 0xdb, 0xc6, 0xb2, 0x2c, 0x2c, 0xcb, 0xa2, 0x14, 0x2a,
	0xa4, 0xeb, 0x20, 0x6c, 0x81, 0x63, 0x0b, 0x8c, 0x31, 0x94, 0x4a, 0x8a,
	0x4b, 0xe7, 0xce, 0x52, 0xb8, 0xd2, 0xdf, 0x55, 0x2c, 0x3a, 0x2d, 0xcf,
	0x3c, 0xf3, 0xf5, 0x9a, 0x20, 0x04, 0xc0, 0xfa, 0xf5, 0x9d, 0xcd, 0x86,
	0xc2, 0x52, 0xbc, 0x0c, 0xf5, 0xf5, 0x39, 0x7c, 0x4f, 0x92, 0x4a, 0xf9,
	0x95, 0xe5, 0xfb, 0x12, 0xdf, 0x97, 0x64, 0x33, 0x01, 0xa1, 0x2a, 0xa3,
	0x42, 0x85, 0x52, 0x11, 0x42, 0x08, 0xb2, 0xd9, 0x34, 0x63, 0x26, 0x4c,
	0xc0, 0xf5, 0x83, 0x46, 0xcb, 0x2a, 0x2c, 0xad, 0x55, 0x01, 0x01, 0x60,
	0x4c, 0x61, 0xbd, 0x08, 0x86, 0x82, 0x07, 0x81, 0x47, 0x2a, 0xf0, 0x49,
	0xa7, 0x03, 0xd2, 0xa9, 0x80, 0x4c, 0x3a, 0x45, 0xb1, 0x18, 0x92, 0x4a,
	0xf9, 0xe4, 0x46, 0x64, 0x70, 0xa5, 0x8b, 0xed, 0xd8, 0x68, 0xad, 0x01,
	0xc8, 0xa6, 0x03, 0x26, 0x4c, 0x9a, 0x84, 0xc1, 0x2c, 0x7a, 0xf9, 0xe5,
	0x4d, 0xcd, 0x35, 0x01, 0xac, 0x5b, 0xf7, 0x6a, 0xab, 0x11, 0xba, 0x21,
	0x37, 0xf2, 0x9a, 0xec, 0xd2, 0x45, 0xca, 0x24, 0xe3, 0xc0, 0xf3, 0x48,
	0x79, 0x92, 0x94, 0x27, 0x91, 0xae, 0x43, 0x18, 0x96, 0xc9, 0xa4, 0x53,
	0x78, 0xd2, 0xc5, 0xbb, 0x76, 0x4e, 0x08, 0x81, 0xb0, 0x13, 0x25, 0x3e,
	0x3d, 0xa9, 0x01, 0xad, 0x4b, 0xcf, 0xd6, 0x04, 0x10, 0xc7, 0xa5, 0x39,
	0x32, 0x95, 0x21, 0x08, 0x3c, 0x6c, 0xdb, 0xc6, 0x75, 0x92, 0x25, 0x1d,
	0x07, 0xe9, 0x38, 0xf8, 0x52, 0xe2, 0x4b, 0x89, 0x2b, 0x5d, 0xb4, 0xd6,
	0x95, 0xe0, 0x9e, 0x37, 0xb4, 0x84, 0x10, 0xb8, 0xd2, 0x65, 0xf4, 0xd8,
	0x3b, 0x51, 0x4a, 0xcd, 0x5d, 0xbd, 0xfa, 0x37, 0xb9, 0x1a, 0x00, 0x8a,
	0xcd, 0xa3, 0xea, 0xeb, 0x11, 0x42, 0x60, 0x8b, 0xe4, 0xb2, 0x0d, 0xde,
	0x7c, 0xdb, 0xb6, 0x2b, 0x59, 0x6a, 0xad, 0x71, 0x5d, 0x17, 0x0b, 0x0b,
	0x57, 0xba, 0xb8, 0x8e, 0x8b, 0x74, 0x1c, 0x5c, 0xdb, 0xc6, 0xb5, 0x6d,
	0x6c, 0xcb, 0x22, 0x95, 0x4a, 0x31, 0x22, 0x37, 0x02, 0x50, 0x8d, 0x55,
	0x03, 0x78, 0x9e, 0x93, 0xb3, 0x6d, 0xfb, 0x13, 0x7d, 0x6e, 0x30, 0xc9,
	0xaf, 0x31, 0x94, 0x42, 0xc5, 0x99, 0xde, 0x8b, 0xb8, 0x8e, 0x9d, 0xc8,
	0xee, 0x26, 0x41, 0x1d, 0x3b, 0x01, 0x1c, 0xbe, 0x1c, 0x5b, 0x50, 0x7f,
	0x47, 0x3d, 0x4a, 0x95, 0xaa, 0x06, 0x70, 0x3c, 0x4f, 0xe2, 0x38, 0x0e,
	0x96, 0x65, 0x81, 0x05, 0xb1, 0x36, 0x9c, 0x3b, 0xdf, 0x47, 0x2a, 0x90,
	0xb8, 0x8e, 0x8b, 0xc1, 0xe0, 0xba, 0x2e, 0x52, 0x3a, 0xd4, 0xa5, 0x53,
	0xc4, 0x71, 0x3c, 0x34, 0x1b, 0x20, 0x79, 0x0e, 0x30, 0xc6, 0xa0, 0x49,
	0x9e, 0x8f, 0x22, 0x55, 0x7d, 0x09, 0x2e, 0x5e, 0xec, 0x23, 0x8e, 0xe3,
	0xa4, 0xd7, 0xb1, 0x70, 0x6c, 0x41, 0x36, 0x13, 0x10, 0x45, 0x9a, 0x28,
	0x8e, 0x92, 0xcc, 0x1c, 0x41, 0xc6, 0xf7, 0xb1, 0x6d, 0x9b, 0x48, 0x6b,
	0x1c, 0x21, 0x70, 0xc4, 0x4d, 0x14, 0x10, 0x02, 0x1d, 0x47, 0x84, 0x61,
	0xb9, 0xea, 0x59, 0xe0, 0xf4, 0xf6, 0xe6, 0x51, 0x4a, 0x21, 0xae, 0x8d,
	0xa4, 0xcb, 0xfd, 0x57, 0x19, 0x51, 0x97, 0x66, 0xd4, 0xa8, 0x3a, 0x84,
	0x25, 0x2a, 0x92, 0x6b, 0xa0, 0x50, 0x2a, 0x55, 0x02, 0x0b, 0x21, 0x92,
	0xfb, 0x72, 0xcd, 0x91, 0x06, 0xb4, 0xd6, 0x9c, 0x3c, 0x71, 0x92, 0x52,
	0x29, 0xea, 0xaa, 0x5a, 0x81, 0x62, 0x31, 0xde, 0x7a, 0xe9, 0xc2, 0xf9,
	0xca, 0xb4, 0x73, 0x1d, 0x9b, 0x42, 0xa1, 0x44, 0xfe, 0xf2, 0x00, 0x57,
	0x0b, 0x45, 0x0a, 0xc5, 0x12, 0x05, 0xa5, 0x88, 0xa2, 0x68, 0x48, 0x7a,
	0x21, 0xb0, 0x2d, 0x2b, 0x51, 0xc2, 0x71, 0x2a, 0xd9, 0xab, 0x50, 0xf1,
	0xe1, 0xb1, 0x0f, 0x09, 0x43, 0x53, 0x3d, 0xc0, 0xd5, 0xab, 0x74, 0x1e,
	0x3b, 0x7c, 0x18, 0x55, 0x8e, 0xc0, 0x02, 0xe9, 0xc9, 0x0a, 0x8c, 0x65,
	0x59, 0x95, 0xd9, 0x3f, 0x58, 0xeb, 0xc1, 0xfd, 0xc1, 0x2e, 0x19, 0xae,
	0xc8, 0xa1, 0x83, 0x87, 0x50, 0x4a, 0x6d, 0x5d, 0xb7, 0xee, 0xa7, 0x55,
	0x97, 0x40, 0xb4, 0xb5, 0xfd, 0xac, 0xed, 0xd4, 0xc9, 0x0b, 0xdd, 0xdd,
	0xc7, 0xbb, 0x31, 0xda, 0x54, 0x6e, 0xb7, 0xb8, 0xe1, 0x3d, 0x49, 0x45,
	0xee, 0x1b, 0x40, 0x2c, 0x8b, 0x52, 0xb1, 0xc4, 0xdb, 0xdb, 0xde, 0xa6,
	0x50, 0x60, 0x6d, 0xb5, 0xc1, 0x2b, 0x3e, 0x8b, 0x45, 0x6f, 0xf1, 0x5f,
	0xf6, 0xbc, 0x4f, 0x3e, 0xdf, 0x0f, 0x40, 0x10, 0x78, 0x95, 0x8c, 0x87,
	0x9b, 0x1e, 0xf6, 0xdf, 0x18, 0x83, 0x31, 0x86, 0x48, 0x6b, 0x62, 0x63,
	0x78, 0xed, 0xb5, 0x37, 0x90, 0xd2, 0x67, 0xe2, 0xc4, 0xcf, 0xce, 0xa9,
	0x19, 0xa0, 0xa3, 0x63, 0xd5, 0xd6, 0xde, 0xf3, 0xf1, 0x2f, 0x76, 0xbc,
	0xd9, 0x49, 0x5f, 0xdf, 0x65, 0x8c, 0x31, 0x48, 0x99, 0x94, 0xc2, 0x68,
	0x83, 0xc1, 0x54, 0x02, 0x1a, 0x93, 0xcc, 0x08, 0xad, 0x35, 0x71, 0x1c,
	0x13, 0xc7, 0x31, 0x1b, 0xda, 0x37, 0xd2, 0xfd, 0x51, 0x37, 0xf7, 0xde,
	0x3b, 0x83, 0xfe, 0xfe, 0x62, 0xd5, 0x2d, 0x08, 0xd7, 0x7d, 0x0f, 0x3c,
	0xfd, 0xf4, 0x73, 0x6b, 0xee, 0x18, 0xe3, 0x2c, 0x6a, 0x79, 0xf4, 0x11,
	0xee, 0x99, 0x32, 0x99, 0xc0, 0xf7, 0x12, 0x89, 0x9d, 0x64, 0xd2, 0x0d,
	0xd6, 0x7c, 0xb0, 0xfe, 0x7d, 0x97, 0xfa, 0xe8, 0xdc, 0xd2, 0x49, 0xef,
	0x85, 0x3e, 0x1e, 0x7c, 0xb0, 0x99, 0x13, 0x27, 0x2e, 0x90, 0xcd, 0xa6,
	0xf2, 0xa3, 0x47, 0x8f, 0x5c, 0x1b, 0xec, 0x5f, 0x34, 0xd1, 0x52, 0xf9,
	0xfc, 0xcc, 0x9f, 0x1c, 0x5a, 0x5c, 0x35, 0x00, 0xc0, 0x82, 0x05, 0x3f,
	0x9a, 0xeb, 0xfb, 0xa5, 0x35, 0x93, 0x26, 0x8f, 0x6e, 0x98, 0xfe, 0xc5,
	0x19, 0x4c, 0xfb, 0xdc, 0x34, 0xa4, 0x4c, 0x5e, 0x46, 0x83, 0x1d, 0x70,
	0xe6, 0x54, 0x0f, 0xfb, 0xff, 0xba, 0x9f, 0x03, 0xfb, 0x0f, 0x60, 0x4c,
	0xba, 0x6d, 0xd6, 0xac, 0x87, 0x5a, 0x8f, 0x1f, 0xef, 0xe9, 0x9e, 0x39,
	0xf3, 0xbe, 0x86, 0x62, 0x31, 0x24, 0xf3, 0xf1, 0x46, 0x72, 0x03, 0xef,
	0x62, 0xca, 0x05, 0x2e, 0x15, 0xb3, 0x6d, 0x0f, 0x2f, 0x3f, 0xfc, 0x9d,
	0xaa, 0x01, 0x06, 0xed, 0xa9, 0xa7, 0x9e, 0x6f, 0x75, 0xdd, 0xf2, 0x42,
	0xc7, 0x89, 0x9a, 0x47, 0x8f, 0x19, 0x49, 0xb6, 0x2e, 0x4d, 0x5c, 0x2e,
	0x73, 0xf6, 0x6c, 0x2f, 0xa5, 0x52, 0xdc, 0x15, 0x45, 0xce, 0xee, 0x72,
	0xd9, 0x5d, 0xdb, 0xd1, 0xb1, 0xaa, 0xfb, 0x85, 0x17, 0x7e, 0xd9, 0xb8,
	0x62, 0xc5, 0x0f, 0xba, 0x56, 0xae, 0x6c, 0xdf, 0x72, 0x9f, 0xb7, 0x67,
	0xee, 0xd8, 0xcc, 0x39, 0x3e, 0x33, 0xd1, 0x47, 0xf5, 0xec, 0xe5, 0xa3,
	0x93, 0x65, 0x2e, 0x5c, 0xad, 0x6b, 0x7b, 0x62, 0xe5, 0xcd, 0x21, 0x6e,
	0x09, 0x30, 0xdc, 0xe6, 0xcf, 0xff, 0x71, 0x03, 0xd0, 0x00, 0xb0, 0x79,
	0xf3, 0x4b, 0xbb, 0x6f, 0x75, 0xee, 0xd5, 0x67, 0xa7, 0x6f, 0x99, 0xd2,
	0x78, 0xcf, 0xdc, 0xc6, 0x6f, 0x7c, 0x9f, 0xf2, 0xe9, 0xbf, 0xa1, 0x7a,
	0xf6, 0xa2, 0x7a, 0xf6, 0x71, 0xf4, 0x5f, 0x65, 0xce, 0x5c, 0xce, 0xb6,
	0xcd, 0x5f, 0x7b, 0x23, 0x84, 0x53, 0x0d, 0xc0, 0xe6, 0xcd, 0x2f, 0x75,
	0x03, 0xdd, 0xff, 0xed, 0x5c, 0xac, 0x0a, 0x0d, 0xb9, 0xa0, 0x17, 0x5b,
	0x9d, 0x84, 0x71, 0x0f, 0x90, 0xe4, 0x67, 0x31, 0x95, 0x7d, 0xa8, 0x23,
	0xfd, 0xad, 0xed, 0xdf, 0x9b, 0xc6, 0xc2, 0x5f, 0x1f, 0xf9, 0x04, 0x84,
	0xb8, 0xde, 0x49, 0xf0, 0x01, 0x66, 0x70, 0x55, 0x03, 0x37, 0xdc, 0x54,
	0x99, 0x96, 0x77, 0x77, 0xf6, 0x74, 0x1d, 0xdd, 0xb9, 0x01, 0x3b, 0x3c,
	0x81, 0x3b, 0x6e, 0x3a, 0x72, 0xfc, 0x97, 0x90, 0xe3, 0x9b, 0xb8, 0x7f,
	0x9a, 0xc7, 0x98, 0xec, 0x95, 0xd6, 0x5f, 0xb5, 0x4e, 0x5b, 0xff, 0x1f,
	0x01, 0x00, 0xde, 0xba, 0xfb, 0x45, 0xf8, 0x73, 0xad, 0xe1, 0xe1, 0xbb,
	0xbf, 0x3d, 0x92, 0x0f, 0x95, 0x69, 0xd9, 0xbe, 0xed, 0x54, 0xd7, 0xc1,
	0xed, 0xed, 0xd8, 0x61, 0x77, 0x02, 0x71, 0x57, 0x13, 0xee, 0x5d, 0x4d,
	0xcc, 0xb8, 0xdf, 0x67, 0xdc, 0x88, 0x2b, 0xad, 0x3f, 0xff, 0xe6, 0xd4,
	0x0a, 0x44, 0x05, 0xe0, 0x66, 0x59, 0x07, 0xab, 0x31, 0xc1, 0xea, 0xda,
	0x94, 0xf8, 0xe1, 0xc6, 0xa3, 0xf9, 0xb0, 0x4c, 0xcb, 0x9b, 0x9d, 0xa7,
	0xbb, 0xfe, 0xbe, 0xad, 0x1d, 0xbb, 0x74, 0x1c, 0x39, 0x36, 0x51, 0xc2,
	0x1d, 0xdf, 0xc4, 0x97, 0x1f, 0x08, 0x98, 0xf0, 0xa9, 0x81, 0xd6, 0x15,
	0xf3, 0x12, 0x88, 0xca, 0x25, 0x0c, 0x3e, 0xc0, 0xbc, 0x75, 0xf7, 0x8b,
	0x37, 0x38, 0x9c, 0xfd, 0xca, 0x72, 0x8a, 0x4b, 0xaa, 0xbb, 0xac, 0xc3,
	0x6d, 0xd9, 0xec, 0xa9, 0xb9, 0x58, 0x9b, 0x5d, 0x73, 0xe7, 0x8d, 0x6d,
	0x9c, 0x3e, 0xfb, 0xdb, 0xe8, 0x60, 0x32, 0xea, 0x74, 0x17, 0xaa, 0x67,
	0x2f, 0xe5, 0x9e, 0xbd, 0xec, 0xdc, 0x53, 0xe4, 0xe0, 0xc7, 0xe9, 0xb6,
	0x21, 0x80, 0xc1, 0x4c, 0x67, 0x26, 0x25, 0x98, 0xfd, 0xca, 0xf2, 0x8a,
	0xb3, 0xff, 0x05, 0x00, 0xe0, 0xf9, 0xaf, 0x4e, 0xc9, 0x19, 0xc3, 0xae,
	0x05, 0x0b, 0xee, 0x6c, 0x9c, 0x31, 0xe7, 0x5b, 0xe8, 0x60, 0x0a, 0xea,
	0xcc, 0x01, 0x54, 0xcf, 0x5e, 0x4e, 0xfd, 0x63, 0x1f, 0xed, 0xdb, 0x83,
	0xa1, 0x12, 0x14, 0x97, 0x60, 0x5d, 0x1f, 0xe8, 0x66, 0x7b, 0xb5, 0xd8,
	0xaa, 0x77, 0xfe, 0x99, 0x37, 0xd0, 0xd2, 0xf1, 0xfb, 0xb3, 0x5d, 0x7f,
	0xea, 0xd8, 0x80, 0x28, 0x1c, 0x45, 0x8e, 0xfd, 0x02, 0x97, 0xcc, 0x64,
	0x36, 0xff, 0xb1, 0x8e, 0x51, 0x4d, 0x0b, 0x6f, 0x74, 0x3e, 0xbc, 0xe6,
	0xff, 0x4f, 0xf0, 0xe1, 0xf6, 0xdc, 0x57, 0xa6, 0xe4, 0x80, 0x5d, 0x33,
	0x9a, 0xea, 0x1b, 0x1f, 0x9a, 0xf3, 0x35, 0x7e, 0xb7, 0x66, 0x3b, 0x9f,
	0x9f, 0xb7, 0x84, 0xed, 0xef, 0x1d, 0xcb, 0xdf, 0x96, 0x00, 0xb5, 0x40,
	0x68, 0xcb, 0x6d, 0x1c, 0xd5, 0xb4, 0x90, 0x13, 0xe7, 0xcb, 0xf9, 0x7c,
	0xfe, 0xca, 0x62, 0xcb, 0x98, 0x9a, 0xdb, 0xfd, 0xb6, 0xda, 0xbf, 0x01,
	0x59, 0x90, 0xa8, 0x1e, 0x1c, 0xc2, 0xb7, 0x4c, 0x00, 0x00, 0x00, 0x00,
	0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};
	
static const unsigned char image1_data[] = { 
	0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7a, 0x7a, 0xf4, 0x00, 0x00, 0x04,
	0xea, 0x49, 0x44, 0x41, 0x54, 0x58, 0x85, 0xb5, 0x57, 0x5f, 0x68, 0x53,
	0x57, 0x18, 0xff, 0x9d, 0x73, 0x4f, 0x4d, 0xd6, 0xa4, 0xb1, 0x74, 0x6d,
	0xac, 0x26, 0xd0, 0x12, 0x42, 0x92, 0x75, 0xa1, 0x98, 0x40, 0x67, 0x0b,
	0xba, 0xd9, 0x22, 0x8c, 0xe9, 0x50, 0x86, 0xb6, 0x0a, 0x85, 0xf9, 0x3c,
	0xb6, 0x37, 0xdf, 0xa4, 0x3e, 0x0e, 0x87, 0x43, 0xfa, 0xb0, 0x27, 0x1d,
	0x7b, 0x90, 0xbd, 0xa9, 0x43, 0x19, 0x9b, 0x58, 0x11, 0x19, 0xb4, 0xf4,
	0xa1, 0x4e, 0x47, 0x1b, 0xd6, 0x4e, 0xb0, 0xc6, 0x06, 0x63, 0xdb, 0xd0,
	0x76, 0x9a, 0xde, 0x24, 0x4d, 0x72, 0xfe, 0xec, 0xa1, 0xbd, 0x77, 0x69,
	0xcc, 0x6d, 0x52, 0xaa, 0x1f, 0x7c, 0xdc, 0x7b, 0xce, 0x3d, 0xf7, 0xfb,
	0x7e, 0xdf, 0x39, 0xdf, 0xbf, 0x43, 0x50, 0x85, 0x06, 0x06, 0x06, 0x2e,
	0x9e, 0x3a, 0x75, 0xea, 0x2b, 0x21, 0x84, 0xe5, 0x1a, 0xa5, 0x14, 0xa4,
	0x94, 0x90, 0x52, 0x42, 0x29, 0x05, 0xc6, 0x18, 0x26, 0x27, 0x27, 0x67,
	0x2f, 0x5d, 0xba, 0x74, 0x04, 0xc0, 0xbf, 0x5b, 0xc9, 0x67, 0xd5, 0x00,
	0xec, 0xdd, 0xbb, 0xd7, 0xd5, 0xdf, 0xdf, 0xbf, 0xbb, 0xda, 0x3a, 0x00,
	0xe0, 0x9c, 0x83, 0x73, 0x0e, 0xbb, 0xdd, 0x8e, 0x4c, 0x26, 0xd3, 0x0e,
	0xc0, 0xbd, 0x63, 0x00, 0x00, 0x64, 0x2d, 0x4a, 0x4b, 0xb9, 0x58, 0x2c,
	0x82, 0x73, 0x2e, 0x00, 0x90, 0x6a, 0xc2, 0xab, 0x02, 0x20, 0xc4, 0x5a,
	0x46, 0x25, 0xe5, 0x9c, 0x73, 0x68, 0x9a, 0x56, 0x4d, 0x6c, 0xed, 0x00,
	0x28, 0xa5, 0xdb, 0x52, 0xce, 0x39, 0x07, 0xa5, 0x74, 0x4b, 0xe0, 0xdb,
	0x02, 0x50, 0xc9, 0x9a, 0xad, 0x94, 0x73, 0xce, 0x6b, 0x56, 0x5e, 0x13,
	0x80, 0xf2, 0x1d, 0xa8, 0xa6, 0xfc, 0xad, 0x03, 0x60, 0xec, 0xff, 0x25,
	0xb5, 0x28, 0x37, 0x00, 0xbc, 0xb5, 0x23, 0x10, 0x42, 0x14, 0x37, 0x9e,
	0x35, 0x03, 0x00, 0x00, 0xa5, 0x94, 0x02, 0x90, 0xab, 0x26, 0x7f, 0x4b,
	0x77, 0x3d, 0x7f, 0xfe, 0xfc, 0x47, 0x5e, 0xaf, 0xf7, 0x44, 0x57, 0x57,
	0x57, 0x90, 0x52, 0x0a, 0x4d, 0xd3, 0x40, 0x29, 0x35, 0x9d, 0xac, 0x12,
	0x03, 0xeb, 0xbb, 0xb6, 0xb8, 0xb8, 0xc8, 0x57, 0x57, 0x57, 0x67, 0xfb,
	0xfa, 0xfa, 0xf4, 0xc7, 0x8f, 0x1f, 0x2f, 0x5b, 0xe9, 0xb0, 0xdc, 0xa7,
	0xbe, 0xbe, 0xbe, 0xde, 0xa1, 0xa1, 0xa1, 0x3b, 0xd7, 0xaf, 0x5f, 0xb7,
	0x77, 0x76, 0x76, 0x42, 0x08, 0x01, 0x21, 0x84, 0x99, 0xf1, 0x0c, 0x36,
	0xe6, 0x4a, 0x9f, 0x00, 0x30, 0x37, 0x37, 0x87, 0x68, 0x34, 0x8a, 0x8e,
	0x8e, 0x8e, 0x17, 0x03, 0x03, 0x03, 0x87, 0xd3, 0xe9, 0xf4, 0x6c, 0x25,
	0x3d, 0x96, 0x47, 0x70, 0xf0, 0xe0, 0xc1, 0x2f, 0x0f, 0x1d, 0x3a, 0x64,
	0xf7, 0x78, 0x3c, 0x28, 0x16, 0x8b, 0xeb, 0x68, 0x4b, 0xce, 0xd5, 0x00,
	0xb4, 0x91, 0x74, 0xde, 0xe0, 0x03, 0x07, 0x0e, 0xa0, 0xa9, 0xa9, 0x09,
	0x4e, 0xa7, 0xd3, 0x1b, 0x0e, 0x87, 0xcf, 0x8c, 0x8f, 0x8f, 0x7f, 0x5b,
	0x33, 0x80, 0x48, 0x24, 0xe2, 0xf7, 0xfb, 0xfd, 0x3d, 0x8c, 0x31, 0x04,
	0x02, 0x81, 0x8a, 0x00, 0xa5, 0x94, 0x28, 0x14, 0x0a, 0x66, 0xe6, 0x33,
	0xb8, 0x74, 0x9c, 0xcf, 0xe7, 0xc1, 0x39, 0x87, 0xdf, 0xef, 0x3f, 0x32,
	0x3e, 0x3e, 0xfe, 0x13, 0x80, 0x85, 0x72, 0x39, 0xa6, 0x49, 0xfd, 0xfd,
	0xfd, 0xad, 0xad, 0xee, 0xd6, 0x93, 0xfb, 0x23, 0xfb, 0x4f, 0x84, 0xc3,
	0xe1, 0x9e, 0xd0, 0x07, 0x21, 0xa7, 0xcb, 0xe5, 0xb2, 0x54, 0x6e, 0x14,
	0x20, 0xa5, 0x94, 0xc9, 0xa5, 0xe3, 0xd2, 0x77, 0x5d, 0xd7, 0x31, 0x33,
	0x33, 0x93, 0x1a, 0x1d, 0x1d, 0xfd, 0xe3, 0xee, 0xdd, 0xbb, 0xbf, 0x3c,
	0x7c, 0xf8, 0xf0, 0x77, 0x00, 0x19, 0x53, 0xe0, 0xbe, 0x7d, 0xfb, 0x02,
	0x37, 0x6e, 0xdc, 0x98, 0x7b, 0xb5, 0xf2, 0x4a, 0x15, 0x8a, 0x05, 0xf5,
	0x2e, 0x29, 0x95, 0x4a, 0xa9, 0xa1, 0xa1, 0xa1, 0x51, 0x00, 0x8d, 0x26,
	0x80, 0xc1, 0xc1, 0xc1, 0x1f, 0x94, 0x52, 0xea, 0x66, 0xfc, 0xa6, 0x3a,
	0xfb, 0xe7, 0x59, 0x75, 0x6d, 0xe6, 0xda, 0x3b, 0x05, 0xb1, 0xb0, 0xb0,
	0xa0, 0x42, 0xa1, 0xd0, 0x37, 0x00, 0x40, 0x01, 0x34, 0x1e, 0x3f, 0x7e,
	0xfc, 0x24, 0x00, 0xe4, 0x8a, 0x39, 0xc4, 0xd2, 0x31, 0x80, 0xae, 0xd7,
	0x78, 0xc3, 0xd1, 0xde, 0x26, 0x2b, 0xa5, 0xb0, 0x67, 0xcf, 0x1e, 0x1c,
	0x3d, 0x7a, 0xf4, 0x0c, 0x00, 0x3b, 0x09, 0x04, 0x02, 0x9f, 0xdd, 0xbe,
	0x7d, 0xfb, 0x4e, 0x30, 0x18, 0x84, 0x2c, 0x4a, 0xac, 0x15, 0xd7, 0x60,
	0xaf, 0xb3, 0x43, 0xc0, 0xba, 0x01, 0xd9, 0x29, 0xd9, 0x6c, 0x36, 0xdc,
	0xba, 0x75, 0x2b, 0x77, 0xfa, 0xf4, 0xe9, 0x6e, 0xe6, 0xf3, 0xf9, 0xa2,
	0x4d, 0x4d, 0x4d, 0xd0, 0x75, 0x1d, 0x1a, 0xd5, 0xa0, 0x51, 0x0d, 0x79,
	0x9e, 0x87, 0x52, 0xea, 0x9d, 0x01, 0x10, 0x42, 0xc0, 0xeb, 0xf5, 0xbe,
	0xd7, 0xda, 0xda, 0x1a, 0x61, 0xed, 0xed, 0xed, 0x6d, 0x85, 0x42, 0x01,
	0x6b, 0x6b, 0x6b, 0xa0, 0x94, 0x82, 0x31, 0x66, 0x66, 0x3c, 0x23, 0xee,
	0x4b, 0xe3, 0xbf, 0x1a, 0xb0, 0x4a, 0x6b, 0xd5, 0x46, 0x54, 0x18, 0xe9,
	0x9c, 0x10, 0x02, 0xa7, 0xd3, 0x09, 0xb7, 0xdb, 0xed, 0x63, 0x76, 0xbb,
	0xdd, 0x61, 0xc4, 0x6f, 0xa9, 0x70, 0x42, 0xc8, 0x1b, 0x69, 0xd7, 0xa8,
	0x8c, 0x56, 0x85, 0xc6, 0xf8, 0xbf, 0x3c, 0x1c, 0x8d, 0xb1, 0x41, 0x86,
	0x81, 0x75, 0x75, 0x75, 0x0e, 0x06, 0x40, 0xe6, 0xf3, 0x79, 0xd3, 0x41,
	0xaa, 0x59, 0x5a, 0xad, 0xca, 0xd5, 0xf2, 0x1f, 0xa5, 0xd4, 0xd8, 0x09,
	0xc2, 0x96, 0x97, 0x97, 0x5f, 0x1b, 0x19, 0x4d, 0x29, 0x05, 0x4a, 0x29,
	0x1c, 0x0e, 0xc7, 0xb6, 0x6a, 0x7a, 0x2d, 0x24, 0x84, 0x40, 0x26, 0xb3,
	0x9e, 0x7b, 0x34, 0x4d, 0x83, 0xae, 0xeb, 0xc8, 0x66, 0xb3, 0x19, 0x96,
	0x48, 0x24, 0x66, 0xb3, 0xd9, 0xac, 0x89, 0x32, 0x9d, 0x4e, 0xe3, 0xea,
	0xd5, 0xab, 0x66, 0x59, 0x2d, 0xdf, 0x99, 0xed, 0x10, 0x21, 0xc4, 0xec,
	0xa8, 0x5a, 0x5a, 0x5a, 0x70, 0xec, 0xd8, 0x31, 0xd3, 0xcf, 0x96, 0x96,
	0x96, 0x90, 0x4a, 0xa5, 0xe6, 0x58, 0x3c, 0x1e, 0x9f, 0x78, 0xf9, 0xf2,
	0xa5, 0x72, 0xbb, 0xdd, 0x84, 0x10, 0x82, 0x95, 0x95, 0x15, 0x5c, 0xb9,
	0x72, 0x65, 0xc7, 0x16, 0x13, 0x42, 0x36, 0x01, 0xf7, 0x78, 0x3c, 0xe8,
	0xed, 0xed, 0x85, 0xcd, 0x66, 0x03, 0x00, 0x3c, 0x7f, 0xfe, 0x7c, 0x75,
	0x7e, 0x7e, 0x7e, 0x86, 0x25, 0x12, 0x89, 0xbf, 0x9e, 0x3c, 0x79, 0x32,
	0xdd, 0xdc, 0xdc, 0xfc, 0x21, 0xe7, 0x1c, 0xf9, 0x7c, 0x7e, 0x93, 0xa0,
	0xe1, 0xe1, 0x61, 0x04, 0x83, 0xc1, 0x37, 0xe6, 0x4b, 0xa9, 0xbc, 0x1c,
	0x0b, 0x21, 0x40, 0x29, 0x45, 0x22, 0x91, 0xc0, 0x85, 0x0b, 0x17, 0x4c,
	0xef, 0xd7, 0x75, 0xdd, 0x74, 0xe4, 0x89, 0x89, 0x89, 0x49, 0x00, 0xff,
	0x30, 0x00, 0xd9, 0x91, 0x91, 0x91, 0x9f, 0xc3, 0xe1, 0xf0, 0x77, 0x0d,
	0x0d, 0x0d, 0x68, 0x6c, 0x6c, 0xc4, 0xb9, 0x73, 0xe7, 0xcc, 0xf6, 0x7a,
	0x70, 0x70, 0x10, 0x2e, 0x97, 0xcb, 0xac, 0xf3, 0xe5, 0x64, 0xd5, 0x15,
	0x49, 0x29, 0x11, 0x0c, 0x06, 0xb1, 0xb8, 0xb8, 0x08, 0x4a, 0x29, 0x6c,
	0x36, 0x1b, 0x08, 0x21, 0x60, 0x8c, 0x21, 0x1e, 0x8f, 0x63, 0x6c, 0x6c,
	0xec, 0x57, 0x00, 0x2b, 0x86, 0xa7, 0x35, 0x74, 0x74, 0x74, 0x7c, 0xdd,
	0xdd, 0xdd, 0xfd, 0x45, 0x57, 0x57, 0x57, 0x38, 0x14, 0x0a, 0xd5, 0x03,
	0x80, 0xcb, 0xe5, 0x42, 0x34, 0x1a, 0xb5, 0xb4, 0x5c, 0x29, 0xb5, 0xa9,
	0x1f, 0x28, 0x7d, 0x37, 0x7a, 0x85, 0xa7, 0x4f, 0x9f, 0xa2, 0xbe, 0xbe,
	0x1e, 0xba, 0xae, 0x63, 0x6a, 0x6a, 0x6a, 0xf5, 0xd1, 0xa3, 0x47, 0x7f,
	0x8f, 0x8d, 0x8d, 0x8d, 0xcc, 0xcf, 0xcf, 0xff, 0x08, 0x20, 0x59, 0xee,
	0xea, 0x6e, 0x4d, 0xd3, 0x3a, 0xdb, 0xda, 0xda, 0x22, 0x3d, 0x3d, 0x3d,
	0x9f, 0x0f, 0x0f, 0x0f, 0x7f, 0xdc, 0xd2, 0xd2, 0x52, 0x93, 0xe5, 0xe5,
	0x8d, 0x89, 0xd1, 0xc4, 0x24, 0x12, 0x09, 0x5c, 0xbe, 0x7c, 0xf9, 0x41,
	0x2c, 0x16, 0xbb, 0x9f, 0x4c, 0x26, 0x63, 0x00, 0xa6, 0x00, 0xbc, 0xc0,
	0xc6, 0x8d, 0xab, 0xfc, 0xd6, 0x91, 0x12, 0x42, 0xdc, 0x7f, 0xf6, 0xec,
	0xd9, 0xf7, 0x3e, 0x9f, 0x2f, 0x6e, 0xa5, 0x1c, 0x58, 0x0f, 0x25, 0x83,
	0x8d, 0x84, 0x55, 0x9e, 0xb8, 0xa4, 0x94, 0xf0, 0x78, 0x3c, 0x90, 0x52,
	0xc6, 0x92, 0xc9, 0xe4, 0x45, 0x00, 0xbf, 0x01, 0x48, 0xa0, 0xe4, 0xba,
	0x57, 0xb1, 0x23, 0x72, 0x38, 0x1c, 0xee, 0x68, 0x34, 0xfa, 0xe9, 0xf4,
	0xf4, 0x34, 0x84, 0x10, 0x9b, 0x62, 0xb0, 0xd4, 0xd1, 0xa4, 0x94, 0xe6,
	0x79, 0x5b, 0xcd, 0xed, 0xda, 0xb5, 0x8b, 0x44, 0x22, 0x91, 0x4f, 0xee,
	0xdd, 0xbb, 0xb7, 0x1b, 0xc0, 0xeb, 0x72, 0x5d, 0x56, 0xd9, 0xc6, 0x6e,
	0xb3, 0xd9, 0x0e, 0x4b, 0x29, 0xdf, 0x07, 0x76, 0x5c, 0x16, 0x29, 0x63,
	0x6c, 0x2d, 0x97, 0xcb, 0x3d, 0x00, 0xf0, 0xaa, 0xfc, 0xe3, 0x7f, 0xc0,
	0x42, 0x83, 0x1b, 0xde, 0xca, 0xf5, 0x81, 0x00, 0x00, 0x00, 0x00, 0x49,
	0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};


class PrintFilter: public QwtPlotPrintFilter{
	public:
	PrintFilter() {};
	
	virtual QColor color(const QColor &c, Item item, int) const{
		if ( !(options() & PrintCanvasBackground)){
			switch(item){
				case MajorGrid:
					return Qt::darkGray ;
				case MinorGrid:
					return Qt::gray ;
				default:
					;
			}
		}
		
		switch(item){
			case Title:
				return Qt::red ;
			case AxisScale:
				return Qt::green ;
			case AxisTitle:
				return Qt::blue ;
			default:
			;
		}
		return c ;
	}
	
	virtual QFont font(const QFont &f, Item, int) const{
		QFont f2 = f ;
		f2.setPointSize(int(f.pointSize() * 1.25)) ;
		return f2 ;
	}
};

DendrogramWidget::DendrogramWidget( QWidget* parent, const char* name, WFlags fl ):
QWidget( parent, name, fl )
{
	
	QImage img;
	img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
	image0 = img;
	img.loadFromData( image1_data, sizeof( image1_data ), "PNG" );
	image1 = img;
	
	if ( !name )
		setName( "FormSpaceDendrogramWidget" );
	FormSpaceDendrogramWidgetLayout = new QGridLayout( this, 1, 1, 20, 6, "FormSpaceDendrogramWidgetLayout"); 

	dendrogram = new Dendrogram( this, "dendrogram" );

	FormSpaceDendrogramWidgetLayout->addMultiCellWidget( dendrogram, 0, 0, 0, 2 );

	pushButtonZoomIn = new QPushButton( this, "pushButtonZoomIn" );
	pushButtonZoomIn->setPixmap( image0 );
	pushButtonZoomIn->setToggleButton( TRUE );

	FormSpaceDendrogramWidgetLayout->addWidget( pushButtonZoomIn, 1, 0 );

	pushButtonPrint = new QPushButton( this, "pushButtonPrint" );
	pushButtonPrint->setPixmap( image1 );

	FormSpaceDendrogramWidgetLayout->addWidget( pushButtonPrint, 1, 1 );
	spacer1 = new QSpacerItem( 430, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	FormSpaceDendrogramWidgetLayout->addItem( spacer1, 1, 2 );
	languageChange();
	resize( QSize(622, 326).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

    // signals and slots connections
	connect( pushButtonPrint, SIGNAL( clicked() ), this, SLOT( print() ) );
	connect( pushButtonZoomIn, SIGNAL( toggled(bool) ), this, SLOT( zoomIn(bool) ) );
	
	/*----------------------------------------------------------------------------------
	* Configuraci� del gr�ico
	*/
	dendrogram->setMinimumSize( 500, 260 ) ;
	
	// Se establece una margen para el componente
	dendrogram->setMargin(5);
	
	// Se desabilitan los grids en x e y
// 	dendrogram->enableGridX(FALSE) ;
// 	dendrogram->enableGridY(FALSE) ;
	
	// Se establece el color de fondo como blanco
	dendrogram->setCanvasBackground(Qt::white) ;
		
	// Seestablece el formato de los numero de los ejes
// 	dendrogram->setAxisLabelFormat(Dendrogram::xBottom, 'f', 1) ;
	
	// Se rotan los valores de los ejes de x, debido a que son muy
	// grandes, con el fin me mejorar su visualizaci�
	dendrogram->setAxisLabelRotation(Dendrogram::xBottom, -20.0) ;
	dendrogram->setAxisLabelAlignment(Dendrogram::xBottom, Qt::AlignLeft | Qt::AlignBottom) ;
		
	// Se inicializa el zoomer
	d_zoomer = new QwtPlotZoomer(Dendrogram::xBottom, Dendrogram::yLeft, QwtPicker::DragSelection, QwtPicker::AlwaysOff, dendrogram->canvas());
	d_zoomer->setRubberBandPen( Qt::green );
		
	// Se inicializa el picker
	d_picker = new QwtPlotPicker(Dendrogram::xBottom, Dendrogram::yLeft, QwtPicker::PointSelection | QwtPicker::DragSelection, 
		QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, dendrogram->canvas());
	d_picker->setRubberBandPen( Qt::green );
// 	d_picker->setCursorLabelPen( Qt::black );
	
	connect( d_picker, SIGNAL( moved(const QPoint &) ), SLOT( moved( const QPoint & ) ) );
	
	zoomIn( FALSE ) ;
}

DendrogramWidget::~DendrogramWidget(){
}

// void DendrogramWidget::setData( QwtArrayData qwtarraydata ){
// 	long id = dendrogram->insertCurve( "Prueba" ) ;
// 	dendrogram->setCurveData(id, qwtarraydata ) ;
// 	
// 	dendrogram->setCurvePen(id, QPen(red));
// 	dendrogram->setCurveStyle( id, QwtCurve::Sticks ) ;
// 	
// 	QwtDendrogramCurve* curve = dendrogram->curve( id ) ;
// 	dendrogram->replot() ;
// 	
// 	d_zoomer->setZoomBase( curve->boundingRect() ) ;
// }

void DendrogramWidget::setXTitle( const QString& text )
{
	dendrogram->setAxisTitle( Dendrogram::xBottom, text ) ;
}

void DendrogramWidget::setYTitle( const QString& text )
{
	dendrogram->setAxisTitle( Dendrogram::yLeft, text ) ;
}

void DendrogramWidget::print(){
// 	#ifndef QT_NO_PRINTER
// 	#if 1
// 		QPrinter printer;
// 	#else
// 		QPrinter printer( QPrinter::HighResolution );
// 		printer.setOutputToFile( TRUE );
// 		printer.setOutputFileName("/tmp/bode.ps");
// 	#endif
// 	
// 	QString docName = dendrogram->title();
// 	if ( docName.isEmpty() ){
// 		docName.replace (QRegExp (QString::fromLatin1 ("\n")), tr (" -- "));
// 		printer.setDocName (docName);
// 	}
// 	
// 	printer.setCreator("Bode example");
// 	printer.setOrientation(QPrinter::Landscape);
// 	
// 	if (printer.setup()){
// 		#if 0
// 			PrintFilter filter;
// 		#else
// 			QwtPlotPrintFilter filter;
// 		#endif
// 		
// 		if ( printer.colorMode() == QPrinter::GrayScale )
// 			filter.setOptions( QwtPlotPrintFilter::PrintAll & ~QwtPlotPrintFilter::PrintCanvasBackground ) ;
// 			
// 		dendrogram->print(printer, filter);
// 	}
// 	#endif
}

void DendrogramWidget::zoomIn( bool zoom ){
	d_zoomer->setEnabled( zoom );
	
	d_zoomer->zoom( 0 ) ;
	
	d_picker->setRubberBand( zoom ? QwtPicker::NoRubberBand: QwtPicker::CrossRubberBand );
}

/**
 * Este metodo sireve para capturar la posici� del mouse, en el futiro
 * incluir�la visualizaci� del grafo correspondiente en un popupmenu
 * @param pos 
 */
void DendrogramWidget::moved(const QPoint &pos){
	QString info ;
	info.sprintf("x = %3.3f y = %3.3f", (double)dendrogram->invTransform(Dendrogram::xBottom, pos.x()), (double)dendrogram->invTransform( Dendrogram::yLeft, pos.y() ) );
}

void DendrogramWidget::languageChange()
{
	setCaption( tr( "Form1" ) );
	QToolTip::add( this, QString::null );
	pushButtonZoomIn->setText( QString::null );
	pushButtonPrint->setText( QString::null );
}

void DendrogramWidget::setEquivalenceClass( QMap< double, Set< Set<string> > > equivalenceClasses, Set<string>* universalSet )
{
	dendrogram->setEquivalenceClass( equivalenceClasses, universalSet ) ;
}


void DendrogramWidget::setMaxRadius( double maxRadius )
{
	dendrogram->setMaxRadius( maxRadius ) ;
}
