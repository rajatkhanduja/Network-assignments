/* Program to generate the following topography. 
 *
 * 
 *    n0 --------- n1 ---------- n2
 *      \                       /
 *       \                     /
 *        \                   /
 *        n3 --------------n4
 *
 *
 * Problem statement :-
 *
 * n0 starts CBR traffic at time 1.0 of rate 900 Kbps destined for n2. n0 starts another CBR traffic at time 1.5 of rate 300 Kbps destined for node 
 * n1. At time 2.0, link from n0 to n1 goes down. Use a dynamic routing protocol so that path n0-n3-n2 is used now At time 2.7, link n0-n1 comes up again.
 * At time 3.0, CBR traffic destined for node n1 stops. CBR destined for n2 stops at time 3.5. Use a  Flow monitor to monitor losses at n2. 
 * Draw a graph of percentage loss as a function of time for the duration of simulation.
 *
 */

#include "ns3/core-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"

#define NODES_N 5


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Topology1");

int
main (int argc, char * argv[])
{
  // 
  // Enable Logs.
  //
  LogComponentEnable ("Topology", LOG_LEVEL_INFO);

  //
  // Create the nodes.
  //
  NS_LOG_INFO ("Create nodes.");
  NodeContainer n;
  n.Create (NODES_N);

  InternetStackHelper internet;
  internet.Install (n);

  
  // 
  // Create the channels required by the topology.
  //
  NS_LOG_INFO ("Create channels.");

  
}
