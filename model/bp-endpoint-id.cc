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

#include<string>
#include "ns3/log.h"
#include "ns3/names.h"
#include "bp-endpoint-id.h"

NS_LOG_COMPONENT_DEFINE ("BpEndpointId"); 

namespace ns3 {


BpEndpointId::BpEndpointId (const std::string scheme, const std::string ssp)
  : m_uri ("")
{ 
  NS_LOG_FUNCTION (this << " " << scheme << " " << ssp);
  ParseComponent (scheme, ssp);
  m_uri = scheme + ":" + ssp;
}

BpEndpointId::BpEndpointId (const std::string uri)
  : m_uri ("")
{ 
  NS_LOG_FUNCTION (this << " " << uri);
  ParseUri (uri);
  m_uri = uri;
}

void 
BpEndpointId::ParseComponent (const std::string &scheme, const std::string &ssp)
{ 
  NS_LOG_FUNCTION (this << " " << scheme << " " << ssp);
  std::string schemeStr = scheme;
  std::string sspStr = ssp;

  size_t schemeLen = schemeStr.length ();
  size_t sspLen = sspStr.length ();

  if (schemeLen == 0 || sspLen == 0)
    {
      NS_LOG_WARN ("BpEndpointId::BuildUri (), both scheme and ssp cannot be empty");
      schemeStr = "dtn";
      sspStr = "none";
      schemeLen = schemeStr.length ();
      sspLen = sspStr.length ();
    }

  // section 4.4 of RFC 5050
  if (schemeLen * sizeof (schemeStr.at (0)) > 1023 / sizeof (char) || 
      sspLen * sizeof (sspStr.at (0)) > 1023 / sizeof (char) )
    {
      NS_LOG_WARN ("BpEndpointId::BuildUri (), both scheme and ssp lengths cannot exceed 1023 bytes");
      schemeStr = schemeStr.substr (0, 1023);
      sspStr = sspStr.substr (0,1023);
      schemeLen = schemeStr.length ();
      sspLen = sspStr.length ();
    }

  // build URI
  m_scheme.offset = 0;
  m_scheme.length = schemeLen;
  m_ssp.offset = schemeLen + 1; 
  m_ssp.length = m_uri.length () - m_ssp.offset;

}

void 
BpEndpointId::ParseUri (const std::string uri)
{ 
  NS_LOG_FUNCTION (this << " " << uri);
  std::string uriStr = uri;
  size_t uriLen = uriStr.length ();

  if (uriLen == 0)
    {
      NS_LOG_WARN ("BpEndpointId::BuildUri (), uri cannot be empty");
      uriStr = "dtn:none";
      uriLen = uriStr.length ();
    }

  size_t semicolon_pos = 0; 
  if((semicolon_pos = uri.find(':')) == std::string::npos)
    {
      NS_LOG_WARN ("BpEndpointId::BuildUri (), uri must include semicolon ':'");
      uriStr = "dtn:none";
      uriLen = uriStr.length ();
    }

  std::string scheme = uriStr.substr(0, semicolon_pos);
  std::string ssp = uriStr.substr(semicolon_pos, uriLen - semicolon_pos);

  ParseComponent (scheme, ssp);
}

std::string
BpEndpointId::Scheme () const
{ 
  NS_LOG_FUNCTION (this);
  return m_uri.substr(m_scheme.offset, m_scheme.length);
}

std::string
BpEndpointId::Ssp () const
{ 
  NS_LOG_FUNCTION (this);
  return m_uri.substr(m_ssp.offset, m_ssp.length);
}

std::string 
BpEndpointId::Uri () const
{ 
  NS_LOG_FUNCTION (this);
  return m_uri;
}

} // namespace ns3
