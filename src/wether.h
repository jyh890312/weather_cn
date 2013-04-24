// wether.h
//
// Copyright (C) 2013 - jyh
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//           wether.h
//  Sun April 07 08:53:26 2013
//  Copyright  2013  jyh
//  <user@host>
#ifndef WETHER_H
#define WETHER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#include <gtk-3.0/gtk/gtk.h>

typedef enum bool{false=0,true=!false} bool;

bool getWebtoXmlFile(char *url);
void writetofile(char *path,char *s,char *mode);
void readfromfile(char *path,char *s);
void paraseXmlWetherInfo(char info[][255],int *type);
//

#endif // WETHER_H
