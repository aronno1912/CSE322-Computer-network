/*
 * Copyright (c) 2013 ResiliNets, ITTC, University of Kansas
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
 * Authors: Siddharth Gangadhar <siddharth@ittc.ku.edu>, Truc Anh N. Nguyen <annguyen@ittc.ku.edu>,
 * and Greeshma Umapathi
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  https://resilinets.org/
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

//============================ just copied from tcpwestwoodplus and made some changes.........................................
#ifndef TCP_ADAPTIVERENO_H
#define TCP_ADAPTIVERENO_H


#include "ns3/data-rate.h"
#include "tcp-congestion-ops.h"
#include "tcp-westwood-plus.h"
#include "ns3/tcp-recovery-ops.h"
#include "ns3/traced-value.h"
#include "ns3/event-id.h"
namespace ns3
{

class Time;
/**
 * \ingroup congestionOps
 *
 * \brief An implementation of TCP Westwood+.
 *
 * Westwood+ employ the AIAD (Additive Increase/Adaptive Decrease)
 * congestion control paradigm. When a congestion episode happens,
 * instead of halving the cwnd, these protocols try to estimate the network's
 * bandwidth and use the estimated value to adjust the cwnd.
 * While Westwood performs the bandwidth sampling every ACK reception,
 * Westwood+ samples the bandwidth every RTT.
 *
 * The two main methods in the implementation are the CountAck (const TCPHeader&)
 * and the EstimateBW (int, const, Time). The CountAck method calculates
 * the number of acknowledged segments on the receipt of an ACK.
 * The EstimateBW estimates the bandwidth based on the value returned by CountAck
 * and the sampling interval (last RTT).
 *
 * WARNING: this TCP model lacks validation and regression tests; use with caution.
 */
class TcpAdaptiveReno : public TcpWestwoodPlus
{
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId();

    TcpAdaptiveReno();
    /**
     * \brief Copy constructor
     * \param sock the object to copy
     */
    TcpAdaptiveReno(const TcpAdaptiveReno& sock);
    ~TcpAdaptiveReno();

    /**
     * \brief Filter type (None or Tustin)
     */
    enum FilterType
    {
        NONE,
        TUSTIN
    };

    //you need to override the existing PktsAcked, GetSsThresh & CongestionAvoidance functions and implement two new ones (EstimateIncWnd and EstimateCongestionLevel).

    uint32_t GetSsThresh(Ptr<const TcpSocketState> tcb, uint32_t bytesInFlight) override;  //need to override
   void PktsAcked(Ptr<TcpSocketState> tcb, uint32_t packetsAcked, const Time& rtt) override; //need to override
   void CongestionAvoidance (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked) override ;
    virtual Ptr<TcpCongestionOps> Fork() ;

  private:
    
    /**
     * Estimate the network's bandwidth
     *
     * \param [in] rtt the RTT estimation.
     * \param [in] tcb the socket state.
     */
    void EstimateBW(const Time& rtt, Ptr<TcpSocketState> tcb);
  double EstimateCongestionLevel();   // new function !!!!!!!!!!!!!!!!!!!!
  void EstimateIncWnd(Ptr<TcpSocketState> tcb); // // new function !!!!!!!!!!!!!!!!!!!!


  protected:
  Time       minRtt;              //!< Minimum RTT
   Time currentRtt ;
   Time jPacketLossRtt ;
   Time jCongestionRtt ;
   Time previousConRtt;
  int32_t                incrementWindow ;                 //!< Increment Window (W_inc)
  uint32_t               baseWindow;                //!< Base Window
  int32_t                probeWindow;         
};

} // namespace ns3

#endif /* TCP_WESTWOOD_H */
