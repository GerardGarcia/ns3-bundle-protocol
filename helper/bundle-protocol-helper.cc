/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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

#include "bundle-protocol-helper.h"
#include "ns3/string.h"
#include "ns3/names.h"
#include "ns3/simulator.h"


namespace ns3 {

BundleProtocolHelper::BundleProtocolHelper ()
  : m_eid ("dtn:none"),
    m_routingProtocol (0)
{
}

BundleProtocolContainer
BundleProtocolHelper::Install (Ptr<Node> node)
{
  return BundleProtocolContainer (InstallPriv (node));
}

BundleProtocolContainer
BundleProtocolHelper::Install (std::string nodeName)
{
  Ptr<Node> node = Names::Find<Node> (nodeName);
  return BundleProtocolContainer (InstallPriv (node));
}

BundleProtocolContainer
BundleProtocolHelper::Install (NodeContainer c)
{
  BundleProtocolContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<BundleProtocol>
BundleProtocolHelper::InstallPriv (Ptr<Node> node)
{
  if (m_eid.Uri () == "dtn:none")
    NS_FATAL_ERROR ("BundleProtocolHelper::InstallPriv (): do not have endpoint id!");
  if (m_routingProtocol == 0)
    NS_FATAL_ERROR ("BundleProtocolHelper::InstallPriv (): do not have bundle routing protocol! " << m_eid.Uri ());

  Ptr<BundleProtocol> bundleProtocol = CreateObject<BundleProtocol> ();
  bundleProtocol->Open (node);   
  bundleProtocol->SetBpEndpointId (m_eid);
  bundleProtocol->SetRoutingProtocol (m_routingProtocol);
  Simulator::Schedule (Seconds (0.0), &BundleProtocol::Initialize, bundleProtocol);

  return bundleProtocol;
}

void 
BundleProtocolHelper::SetBpEndpointId (BpEndpointId eid)
{
  m_eid = eid;
}

void 
BundleProtocolHelper::SetRoutingProtocol (Ptr<BpRoutingProtocol> rt)
{
  m_routingProtocol = rt;
}


} // namespace ns3
