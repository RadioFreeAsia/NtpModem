// ntpmodemd.cpp
//
// ntpmodemd(8) time service daemon
//
//   (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <poll.h>
#include <stdio.h>

#include <QCoreApplication>

#include "ntpmodemd.h"

MainObject::MainObject(QObject *parent)
  : QObject(parent)
{
  QDateTime now=QDateTime::currentDateTime();
  struct pollfd fds;

  memset(&fds,0,sizeof(fds));
  while(1==1) {
    poll(&fds,0,now.msecsTo(NextTick(now)));
    now=QDateTime::currentDateTime();
    printf("%d %d %s UTC \r\n",now.date().toJulianDay(),now.date().dayOfYear(),
	   (const char *)now.toString("hhmmss").toUtf8());
    printf("*\r\n");
  }
}


QDateTime MainObject::NextTick(const QDateTime &dt) const
{
  QDateTime ret=dt.addSecs(1);
  ret=QDateTime(ret.date(),QTime(ret.time().hour(),ret.time().minute(),ret.time().second(),0));

  return ret;
}


int MainObject::JulianDay(const QDate &date)
{
  //
  // See https://en.wikipedia.org/wiki/julian_day#cite_ref-28 for a
  // description of this algorithm.
  //
  int a=(14-date.month())/12;
  int y=date.year()+4800-a;
  int m=date.month()+12*a-3;

  return date.day()+((153*m+2)/5)+(365*y)+(y/4)-(y/100)+(y/400)-32045;
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);
  new MainObject();
  return a.exec();
}
