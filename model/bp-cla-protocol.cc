/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of New Brunswick
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Dizhi Zhou <dizhi.zhou@gmail.com>
 */

#include "bp-cla-protocol.h"
#include "ns3/socket.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("BpClaProtocol");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (BpClaProtocol);

TypeId 
BpClaProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BpClaProtocol")
    .SetParent<Object> ()
  ;
  return tid;
}


BpClaProtocol::BpClaProtocol ()
{
  NS_LOG_FUNCTION (this);
}

BpClaProtocol::~BpClaProtocol ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3

