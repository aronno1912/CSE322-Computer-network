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
 * Authors: Siddharth Gangadhar <siddharth@ittc.ku.edu>,
 *          Truc Anh N. Nguyen <annguyen@ittc.ku.edu>,
 *          Greeshma Umapathi
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

// ============================copied the file tcpWestwoodPlus and made changes there ============================================
#include "tcp-adaptive-reno.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "rtt-estimator.h"
#include "tcp-socket-base.h"


NS_LOG_COMPONENT_DEFINE("TcpAdaptiveReno");

namespace ns3
{

NS_OBJECT_ENSURE_REGISTERED(TcpAdaptiveReno);

TypeId
TcpAdaptiveReno::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::TcpAdaptiveReno")
            .SetParent<TcpNewReno>()
            .SetGroupName("Internet")
            .AddConstructor<TcpAdaptiveReno>()
            .AddAttribute(
                "FilterType",
                "Use this to choose no filter or Tustin's approximation filter",
                EnumValue(TcpAdaptiveReno::TUSTIN),
                MakeEnumAccessor(&TcpAdaptiveReno::m_fType),
                MakeEnumChecker(TcpAdaptiveReno::NONE, "None", TcpAdaptiveReno::TUSTIN, "Tustin"))
            .AddTraceSource("EstimatedBW",
                            "The estimated bandwidth",
                            MakeTraceSourceAccessor(&TcpAdaptiveReno::m_currentBW),
                            "ns3::TracedValueCallback::Double");
    return tid;
}


TcpAdaptiveReno::TcpAdaptiveReno()
    : TcpWestwoodPlus(),
  minRtt (Time (0)),
  currentRtt (Time (0)),
  jPacketLossRtt (Time (0)),
  jCongestionRtt (Time (0)),
  previousConRtt (Time(0)),
  incrementWindow (0),
  baseWindow(0),
  probeWindow (0)
{
    NS_LOG_FUNCTION(this);
}

TcpAdaptiveReno::TcpAdaptiveReno(const TcpAdaptiveReno& sock)
      : TcpWestwoodPlus(sock),
  minRtt (Time (0)),
  currentRtt (Time (0)),
  jPacketLossRtt (Time (0)),
  jCongestionRtt (Time (0)),
  previousConRtt (Time(0)),
  incrementWindow (0), 
  baseWindow(0),
  probeWindow (0)
{
    NS_LOG_FUNCTION(this);
    NS_LOG_LOGIC("Invoked the copy constructor");
}

TcpAdaptiveReno::~TcpAdaptiveReno()
{
}
//==============override=============================
/****
 * This function is called each time a packet is Acked. It will increase the
count of acked segments and update the current estimated bandwidth.
*/
void
TcpAdaptiveReno::PktsAcked(Ptr<TcpSocketState> tcb, uint32_t packetsAcked, const Time& rtt)
{
    NS_LOG_FUNCTION(this << tcb << packetsAcked << rtt);

    if (rtt.IsZero())
    {
        NS_LOG_WARN("RTT measured is zero!");
        return;
    }

    m_ackedSegments += packetsAcked;

    // if (!(rtt.IsZero() || m_IsCount))
    // {
    //     m_IsCount = true;
    //     m_bwEstimateEvent.Cancel();
    //     m_bwEstimateEvent = Simulator::Schedule(rtt, &TcpAdaptiveReno::EstimateBW, this, rtt, tcb);
    // }
   

   //This helps track the minimum RTT observed.
    if(minRtt.IsZero())
    {
        minRtt=rtt;
    }
    if(rtt<=minRtt)
    {
        minRtt=rtt;
    }
    TcpWestwoodPlus::EstimateBW (rtt, tcb);
}

double
TcpAdaptiveReno::EstimateCongestionLevel()
{
  /***
   * Estimates the current congestion level using Round
Trip Time (RTT). First, you need to calculate the RTT cong , the value RTT is estimated
to take when a packet is lost due to congestion
  */

  
   //===================== RTT_cong =alpha* prev_RTT_cong + (1-alpha)*RTT
  
   float alpha=0.85;
   if(previousConRtt<minRtt)
   {
    alpha=0;
   }
   double newValjconRtt= alpha*previousConRtt.GetSeconds() +(1-alpha)*jPacketLossRtt.GetSeconds();
   jCongestionRtt=Seconds(newValjconRtt);
   // formula===============
   double compare=(currentRtt.GetSeconds()-minRtt.GetSeconds()) / (jCongestionRtt.GetSeconds() - minRtt.GetSeconds());
   return std::min(compare,1.0);

}


void 
TcpAdaptiveReno::EstimateIncWnd(Ptr<TcpSocketState> tcb)
{
  
  double c = EstimateCongestionLevel();
  int M = 1000;
  //B is the current bandwidth calculated in TcpWestWoodPlus’s
  //EstimateBW() function which is called in PktsAcked(). stored in currentBw
  DataRate currentBandwidth = m_currentBW; 
  double B = currentBandwidth.GetBitRate();
  //MSS is the maximum segment size which is the square of the original SegmentSize.
  double MSS=static_cast<double> (tcb->m_segmentSize * tcb->m_segmentSize) ;
  double w_max_inc = B/M*MSS; 
  double alpha=10; 
  double beta = 2 * w_max_inc * ((1/alpha) - ((1/alpha + 1)/(std::exp(alpha))));
  double gamma = 1 - (2 * w_max_inc * ((1/alpha) - ((1/alpha + 0.5)/(std::exp(alpha)))));
  //Calculate w_inc
   incrementWindow = (int)((w_max_inc / std::exp(alpha * c)) + (beta * c) + gamma);
}

void
TcpAdaptiveReno::CongestionAvoidance (Ptr<TcpSocketState> tcb, uint32_t segmentsAcked)
{
  /*
 The base part maintains a congestion window size equivalent to TCP-Reno and the probing part is
introduced to quickly fill the bottleneck link. The base part is always increased like
TCP-Reno, i.e. 1MSS/RTT, while the increase of the probing part, Winc , is dynamically
adjusted

  */

  NS_LOG_FUNCTION (this << tcb << segmentsAcked);
  if (segmentsAcked > 0)
    {
        EstimateIncWnd(tcb);
      // basewindow = TCP NEW RENO IMPLEMENTATION
      //=================== copied from implementation of tcpNewReno=====================================
      double adder = static_cast<double> (tcb->m_segmentSize * tcb->m_segmentSize) / tcb->m_cWnd.Get ();
      adder = std::max (1.0, adder);
      baseWindow += static_cast<uint32_t> (adder);

      // Wprobe = max(Wprobe + Winc /W, 0)
        probeWindow = std::max((double) (probeWindow + incrementWindow / (int)tcb->m_cWnd.Get()), (double) 0);
      // NS_LOG_UNCOND("Congestion window changed");
      NS_LOG_INFO ("In CongAvoid, updated to cwnd " << tcb->m_cWnd <<
                   " ssthresh " << tcb->m_ssThresh);
    }

}
//====================This function is called when a loss event has occured============================
uint32_t
TcpAdaptiveReno::GetSsThresh (Ptr<const TcpSocketState> tcb,uint32_t bytesInFlight)
{
  

 /**** uint32_t ssThresh = static_cast<uint32_t>((m_currentBW * tcb->m_minRtt) / 8.0);

    NS_LOG_LOGIC("CurrentBW: " << m_currentBW << " minRtt: " << tcb->m_minRtt
                               << " ssThresh: " << ssThresh);

    return std::max(2 * tcb->m_segmentSize, ssThresh);*/

    /*

      new_window = current_window / (1 + c)

  */

  // update the prevRTTCon and jRtt(jpacketlossrtt)
  previousConRtt = jCongestionRtt; // a loss event has occured,so set the previous conjestion RTT
  jPacketLossRtt = currentRtt; // this will now contain the RTT of previous packet or jth loss event
  
  double c = EstimateCongestionLevel();
  //************************Note: the minimum value the ssthresh can take is 2*segmentSize similar to the other algorithms********************
  uint32_t thresoldVal = std::max (2*tcb->m_segmentSize, (uint32_t) (tcb->m_cWnd / (1.0+c)));
  //update values
  baseWindow = thresoldVal;
  probeWindow = 0; //for next calculation
  
  NS_LOG_LOGIC("new ssthresh : "<<thresoldVal<<" ; old conj Rtt : "<<previousConRtt<<" ; new conj Rtt : "<<jCongestionRtt<<" ; cong : "<<c);
  return thresoldVal;

}


Ptr<TcpCongestionOps>
TcpAdaptiveReno::Fork()
{
    return CreateObject<TcpAdaptiveReno>(*this);
}

} // namespace ns3
