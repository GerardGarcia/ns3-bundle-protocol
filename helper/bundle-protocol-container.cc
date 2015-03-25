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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Modified by Dizhi Zhou <dizhi.zhou@gmail.com>
 */

#include "ns3/names.h"
#include "bundle-protocol-container.h"

namespace ns3 {

BundleProtocolContainer::BundleProtocolContainer ()
{
}

BundleProtocolContainer::BundleProtocolContainer (Ptr<BundleProtocol> bundleProtocol)
{
  m_bundleProtocols.push_back (bundleProtocol);
}

BundleProtocolContainer::BundleProtocolContainer (std::string name)
{
  Ptr<BundleProtocol> bundleProtocol = Names::Find<BundleProtocol> (name);
  m_bundleProtocols.push_back (bundleProtocol);
}


BundleProtocolContainer::Iterator 
BundleProtocolContainer::Begin (void) const
{
  return m_bundleProtocols.begin ();
}
BundleProtocolContainer::Iterator 
BundleProtocolContainer::End (void) const
{
  return m_bundleProtocols.end ();
}

uint32_t 
BundleProtocolContainer::GetN (void) const
{
  return m_bundleProtocols.size ();
}

Ptr<BundleProtocol> 
BundleProtocolContainer::Get (uint32_t i) const
{
  return m_bundleProtocols[i];
}

void 
BundleProtocolContainer::Add (BundleProtocolContainer other)
{
  for (Iterator i = other.Begin (); i != other.End (); i++)
    {
      m_bundleProtocols.push_back (*i);
    }
}
void 
BundleProtocolContainer::Add (Ptr<BundleProtocol> bundleProtocol)
{
  m_bundleProtocols.push_back (bundleProtocol);
}
void 
BundleProtocolContainer::Add (std::string name)
{
  Ptr<BundleProtocol> bundleProtocol = Names::Find<BundleProtocol> (name);
  m_bundleProtocols.push_back (bundleProtocol);
}

void 
BundleProtocolContainer::Start (Time start)
{
  for (Iterator i = Begin (); i != End (); ++i)
    {
      Ptr<BundleProtocol> bp = *i;
      bp->SetStartTime (start);
    }
}

void 
BundleProtocolContainer::Stop (Time stop)
{
  for (Iterator i = Begin (); i != End (); ++i)
    {
      Ptr<BundleProtocol> bp = *i;
      bp->SetStopTime (stop);
    }
}


} // namespace ns3
