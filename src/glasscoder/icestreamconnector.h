// icestreamconnector.h
//
// Glasscoder connector for an integrated IceCast server
//
//   (C) Copyright 2014-2016 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef ICESTREAMCONNECTOR_H
#define ICESTREAMCONNECTOR_H

#include <vector>

#include <QSignalMapper>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

#include "connector.h"

class IceStream
{
 public:
  IceStream(QTcpSocket *sock);
  ~IceStream();
  QTcpSocket *socket() const;
  bool isNegotiated() const;
  void setNegotiated();
  QString mountpoint() const;
  void setMountpoint(const QString &str);
  QString accum;

 private:
  unsigned ice_id;
  QTcpSocket *ice_socket;
  bool ice_is_negotiated;
  QString ice_mountpoint;
};




class IceStreamConnector : public Connector
{
  Q_OBJECT;
 public:
  IceStreamConnector(QObject *parent=0);
  ~IceStreamConnector();
  Connector::ServerType serverType() const;

 private slots:
  void newConnectionData();
  void readyReadData(int id);
  void disconnectedData();
  void garbageData();

 protected:
  void connectToHostConnector(const QString &hostname,uint16_t port);
  void disconnectFromHostConnector();
  int64_t writeDataConnector(int frames,const unsigned char *data,int64_t len);

 private:
  void SendHeader(IceStream *strm,const QString &hdr="") const;
  void ProcessHeader(IceStream *strm);
  void DenyConnection(IceStream *strm,int code,const QString &str);
  QTcpServer *iceserv_server;
  std::vector<IceStream *> iceserv_streams;
  QSignalMapper *iceserv_readyread_mapper;
  QTimer *iceserv_garbage_timer;
};


#endif  // ICESTREAMCONNECTOR_H
