/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2015,  Arizona Board of Regents.
 *
 * This file is part of ndn-tools (Named Data Networking Essential Tools).
 * See AUTHORS.md for complete list of ndn-tools authors and contributors.
 *
 * ndn-tools is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndn-tools is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndn-tools, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Eric Newberry <enewberry@email.arizona.edu>
 * @author Jerald Paul Abraham <jeraldabraham@email.arizona.edu>
 */

#include "ping-server.hpp"

namespace ndn {
namespace ping {
namespace server {

PingServer::PingServer(Face& face, const Options& options)
  : m_options(options)
  , m_name(options.prefix)
  , m_nPings(0)
  , m_face(face)
  , m_firstInterest(true)
  , m_currentDataNum(0)
{
  shared_ptr<Buffer> b = make_shared<Buffer>();
  b->assign(m_options.payloadSize, 'a');
  m_payload = Block(tlv::Content, b);
}

void
PingServer::run()
{
  start();

  m_face.getIoService().run();
}

void
PingServer::start()
{
  m_name.append("ping");
  m_face.setInterestFilter(m_name,
                           bind(&PingServer::onInterest,
                                this, _2),
                           bind(&PingServer::onRegisterFailed,
                                this, _2));
}

int
PingServer::getNPings() const
{
  return m_nPings;
}

void
PingServer::onInterest(const Interest& interest)
{
  Name interestName = interest.getName();

  afterReceive(interestName);

  shared_ptr<Data> data;
  
  // if data pre-generated
  if (m_options.preGenerate) {
    // populate data array if first time
    if (m_firstInterest) {
      std::cout << "populate data array" << std::endl;
      generateDataPackets(interestName);
      m_firstInterest = false;
    }
    // get data out of array
    std::cout << "from array" << std::endl;
    data = m_dataArray[m_currentDataNum];
    ++m_currentDataNum;
  }
  // make and sign data at runtime
  else {
    std::cout << "made at runtime" << std::endl;
    data = make_shared<Data>(interestName);
    data->setFreshnessPeriod(m_options.freshnessPeriod);
    data->setContent(m_payload);
    m_keyChain.sign(*data); //, signingWithSha256());
  }
  
  m_face.put(*data);

  ++m_nPings;
  if (m_options.shouldLimitSatisfied && m_options.nMaxPings > 0 && m_options.nMaxPings == m_nPings) {
    m_face.shutdown();
    m_face.getIoService().stop();
  }
}

void
PingServer::onRegisterFailed(const std::string& reason)
{
  throw "Failed to register prefix in local hub's daemon, REASON: " + reason;
}

void
PingServer::generateDataPackets(Name interestName)
{
  // split interest name into prefix and ping sequence number
  std::string name = interestName.toUri();
  size_t delimiter = name.find_last_of("/");
  std::string prefix = name.substr(0,delimiter);
  std::string seqStr = name.substr(delimiter+1);
  uint64_t seq = std::stoull(seqStr);

  for (int i = 0; i < m_nDataPackets; i++) 
  {
    std::cout << i << std::endl;

    Name dataName = Name(prefix);
    dataName.append(std::to_string(seq)); // add sequence number to name
    
    shared_ptr<Data> data = make_shared<Data>(dataName);
    data->setFreshnessPeriod(m_options.freshnessPeriod);
    data->setContent(m_payload);
    m_keyChain.sign(*data);

    m_dataArray[i] = data;

    ++seq;
  }
}

} // namespace server
} // namespace ping
} // namespace ndn