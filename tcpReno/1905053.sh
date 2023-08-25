# #!/bin/bash
rm -f GeneratedFiles/tcphs/*.dat
rm -f GeneratedFiles/tcphs/*.cwnd
rm -f GeneratedFiles/TcpAdaptiveReno/*.dat
rm -f GeneratedFiles/TcpAdaptiveReno/*.cwnd
rm -f GeneratedFiles/TcpWestwoodPlus/*.dat
rm -f GeneratedFiles/TcpWestwoodPlus/*.cwnd


# ########+++++++++++++++++++++++++++++++++++++++++++++++++TCP high speed+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# # #########################################   Varying bottleneckDataRate    #####################################################################################

# declare -a bottleNeckarray=("1" "50" "100" "200" "300")

# folder_name="tcphs"
# for nF in "${bottleNeckarray[@]}"
# do
#     ./ns3 run scratch/1905053 -- --bottleneckDataRate=$nF --packet_loss_exp=6 --dataTrack=1 --algo2="ns3::TcpHighSpeed" --folder_name="$folder_name"
    
#     sleep 5

#     # Generate graph for each flow's Congestion Window vs. Time
#     gnuplot -persist <<-EOFMarker
#         set terminal png size 640,480
#         set output "GeneratedFiles/$folder_name/cwndForBottle_${nF}.png"
#         set xlabel "Time (s)"
#         set ylabel "Congestion Window"
#         plot "GeneratedFiles/$folder_name/flow1.cwnd" with lines title "TCPNewReno", \
#              "GeneratedFiles/$folder_name/flow2.cwnd" with lines title "TCPHighSpeed"
# EOFMarker
# done


# #Plot the graph using Gnuplot
# gnuplot -persist <<-EOFMarker
#     set terminal png size 640,480
#     set output "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate.png"
#     set xlabel "bottoleNeckDataRate(Mbps)"
#     set ylabel "Throughput (Kbit/s)"
#      plot "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate1.dat" with linespoints title "TcpNewReno" lc rgb 'blue', \
#          "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate2.dat" with linespoints title "TcpHighSpeed" lc rgb 'red'
# EOFMarker
# ##+++++++++++++++++   fairness index plot+++++++++++++++++++++++++++++++++
# gnuplot -persist <<-EOFMarker
#     set terminal png size 640,480
#     set output "GeneratedFiles/$folder_name/FairnessIndex_vs_BottleNeckRate.png"
#     set xlabel "BottleNeckRate"
#     set ylabel "Fairness index"
#     plot "GeneratedFiles/$folder_name/FairnessIndex_vs_BottleNeckRate.dat" with linespoints title "TcpNewReno+$folder_name"
# EOFMarker


# ##########################################   Varying packetlossRate    #####################################################################################

# declare -a packExp=("2" "3" "4" "5" "6")

# # Loop over each value of nFlows and run the simulation
# for nF in "${packExp[@]}"
# do
#     ./ns3 run scratch/1905053 -- --bottleneckDataRate=50 --packet_loss_exp=$nF --dataTrack=2 --algo2="ns3::TcpHighSpeed" --folder_name="$folder_name"
#     sleep 5
#     gnuplot -persist <<-EOFMarker
#         set terminal png size 640,480
#         set output "GeneratedFiles/$folder_name/cwndForPacketLoss_${nF}.png"
#         set xlabel "Time (s)"
#         set ylabel "Congestion Window"
#         plot "GeneratedFiles/$folder_name/flow1.cwnd" with lines title "TCPNewReno", \
#              "GeneratedFiles/$folder_name/flow2.cwnd" with lines title "TCPHighSpeed"
# EOFMarker
# done


# #Plot the graph using Gnuplot
# gnuplot -persist <<-EOFMarker
#     set terminal png size 640,480
#     set output "GeneratedFiles/$folder_name/throughput_vs_packetLossrate.png"
#     set xlabel "PacketLossRate"
#     set ylabel "Throughput (Kbit/s)"
#      set logscale x  # Set x-axis to logarithmic scale
#     set xrange [1e-6:1e-2]  # Set the x-axis range from 10^-6 to 10^-2
#      plot "GeneratedFiles/$folder_name/throughput_vs_packetLossRatio1.dat" with linespoints title "TcpNewReno" lc rgb 'blue', \
#          "GeneratedFiles/$folder_name/throughput_vs_packetLossRatio2.dat" with linespoints title "TcpHighSpeed" lc rgb 'red'
# EOFMarker
# ##+++++++++++++++++ fairness index plot+++++++++++++++++++++++++++++++++
# gnuplot -persist <<-EOFMarker
#     set terminal png size 640,480
#     set output "GeneratedFiles/$folder_name/FairnessIndex_vs_PacketLossRate.png"
#     set xlabel "PacketLossRate"
#     set ylabel "Fairness index"
#     plot "GeneratedFiles/$folder_name/FairnessIndex_vs_PacketLossRate.dat" with linespoints title "TcpNewReno+$folder_name"
# EOFMarker












##########################+++++++++++++++++++++++++++++++Adaptive Reno+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++==========================================================

######## Varying bottleneckRate
declare -a bottleNeckarray=("1" "50" "100" "200" "300")

# Loop over each value of nFlows and run the simulation
folder_name="TcpAdaptiveReno"
for nF in "${bottleNeckarray[@]}"
do
    ./ns3 run scratch/1905053 -- --bottleneckDataRate=$nF --packet_loss_exp=6 --dataTrack=1 --algo2="ns3::TcpAdaptiveReno" --folder_name="$folder_name"
    
    sleep 5

    # Generate graph for each flow's Congestion Window vs. Time
    gnuplot -persist <<-EOFMarker
        set terminal png size 640,480
        set output "GeneratedFiles/$folder_name/cwndForBottle_${nF}.png"
        set xlabel "Time (s)"
        set ylabel "Congestion Window"
        plot "GeneratedFiles/$folder_name/flow1.cwnd" with lines title "TCPNewReno", \
             "GeneratedFiles/$folder_name/flow2.cwnd" with lines title "TcpAdaptiveReno"
EOFMarker
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate.png"
    set xlabel "bottoleNeckDataRate(Mbps)"
    set ylabel "Throughput (Kbit/s)"
     plot "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate1.dat" with linespoints title "TcpNewReno" lc rgb 'blue', \
         "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate2.dat" with linespoints title "TcpAdaptiveReno" lc rgb 'red'
EOFMarker
##+++++++++++++++++ fairness index plot+++++++++++++++++++++++++++++++++
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "GeneratedFiles/$folder_name/FairnessIndex_vs_BottleNeckRate.png"
    set xlabel "BottleNeckRate"
    set ylabel "Fairness index"
    plot "GeneratedFiles/$folder_name/FairnessIndex_vs_BottleNeckRate.dat" with linespoints title "TcpNewReno+$folder_name"
EOFMarker
##########################################   Varying packetlossRate    #####################################################################################

declare -a packExp=("2" "3" "4" "5" "6")

# Loop over each value of nFlows and run the simulation
for nF in "${packExp[@]}"
do
    ./ns3 run scratch/1905053 -- --bottleneckDataRate=50 --packet_loss_exp=$nF --dataTrack=2 --algo2="ns3::TcpAdaptiveReno" --folder_name="$folder_name"
    sleep 5
    gnuplot -persist <<-EOFMarker
        set terminal png size 640,480
        set output "GeneratedFiles/$folder_name/cwndForPacketLoss_${nF}.png"
        set xlabel "Time (s)"
        set ylabel "Congestion Window"
        plot "GeneratedFiles/$folder_name/flow1.cwnd" with lines title "TCPNewReno", \
             "GeneratedFiles/$folder_name/flow2.cwnd" with lines title "TcpAdaptiveReno"
EOFMarker
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "GeneratedFiles/$folder_name/throughput_vs_packetLossrate.png"
    set xlabel "PacketLossRate"
    set ylabel "Throughput (Kbit/s)"
     set logscale x  # Set x-axis to logarithmic scale
    set xrange [1e-6:1e-2]  # Set the x-axis range from 10^-6 to 10^-2
     plot "GeneratedFiles/$folder_name/throughput_vs_packetLossRatio1.dat" with linespoints title "TcpNewReno" lc rgb 'blue', \
         "GeneratedFiles/$folder_name/throughput_vs_packetLossRatio2.dat" with linespoints title "TcpAdaptiveReno" lc rgb 'red'
EOFMarker
##+++++++++++++++++ fairness index plot+++++++++++++++++++++++++++++++++
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "GeneratedFiles/$folder_name/FairnessIndex_vs_PacketLossRate.png"
    set xlabel "PacketLossRate"
    set ylabel "Fairness index"
    plot "GeneratedFiles/$folder_name/FairnessIndex_vs_PacketLossRate.dat" with linespoints title "TcpNewReno+$folder_name"
EOFMarker













# ##########################+++++++++++++++++++++++++++++++WestWOOdPLUS+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++==========================================================

# ######## Varying bottleneckRate
# declare -a bottleNeckarray=("1" "50" "100" "200" "300")

# # Loop over each value of nFlows and run the simulation
# folder_name="TcpWestwoodPlus"
# for nF in "${bottleNeckarray[@]}"
# do
#     ./ns3 run scratch/1905053 -- --bottleneckDataRate=$nF --packet_loss_exp=6 --dataTrack=1 --algo2="ns3::TcpWestwoodPlus" --folder_name="$folder_name"
    
#     sleep 5

#     # Generate graph for each flow's Congestion Window vs. Time
#     gnuplot -persist <<-EOFMarker
#         set terminal png size 640,480
#         set output "GeneratedFiles/$folder_name/cwndForBottle_${nF}.png"
#         set xlabel "Time (s)"
#         set ylabel "Congestion Window"
#         plot "GeneratedFiles/$folder_name/flow1.cwnd" with lines title "TCPNewReno", \
#              "GeneratedFiles/$folder_name/flow2.cwnd" with lines title "TcpWestwoodPlus"
# EOFMarker
# done


# #Plot the graph using Gnuplot
# gnuplot -persist <<-EOFMarker
#     set terminal png size 640,480
#     set output "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate.png"
#     set xlabel "bottoleNeckDataRate(Mbps)"
#     set ylabel "Throughput (Kbit/s)"
#      plot "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate1.dat" with linespoints title "TcpNewReno" lc rgb 'blue', \
#          "GeneratedFiles/$folder_name/throughput_vs_bottoleNeckDataRate2.dat" with linespoints title "TcpWestwoodPlus" lc rgb 'red'
# EOFMarker
# ##+++++++++++++++++ fairness index plot+++++++++++++++++++++++++++++++++
# gnuplot -persist <<-EOFMarker
#     set terminal png size 640,480
#     set output "GeneratedFiles/$folder_name/FairnessIndex_vs_BottleNeckRate.png"
#     set xlabel "BottleNeckRate"
#     set ylabel "Fairness index"
#     plot "GeneratedFiles/$folder_name/FairnessIndex_vs_BottleNeckRate.dat" with linespoints title "TcpNewReno+$folder_name"
# EOFMarker
# ##########################################   Varying packetlossRate    #####################################################################################

# declare -a packExp=("2" "3" "4" "5" "6")

# # Loop over each value of nFlows and run the simulation
# for nF in "${packExp[@]}"
# do
#     ./ns3 run scratch/1905053 -- --bottleneckDataRate=50 --packet_loss_exp=$nF --dataTrack=2 --algo2="ns3::TcpWestwoodPlus" --folder_name="$folder_name"
#     sleep 5
#     gnuplot -persist <<-EOFMarker
#         set terminal png size 640,480
#         set output "GeneratedFiles/$folder_name/cwndForPacketLoss_${nF}.png"
#         set xlabel "Time (s)"
#         set ylabel "Congestion Window"
#         plot "GeneratedFiles/$folder_name/flow1.cwnd" with lines title "TCPNewReno", \
#              "GeneratedFiles/$folder_name/flow2.cwnd" with lines title "TcpWestwoodPlus"
# EOFMarker
# done


# #Plot the graph using Gnuplot
# gnuplot -persist <<-EOFMarker
#     set terminal png size 640,480
#     set output "GeneratedFiles/$folder_name/throughput_vs_packetLossrate.png"
#     set xlabel "PacketLossRate"
#     set ylabel "Throughput (Kbit/s)"
#      set logscale x  # Set x-axis to logarithmic scale
#     set xrange [1e-6:1e-2]  # Set the x-axis range from 10^-6 to 10^-2
#      plot "GeneratedFiles/$folder_name/throughput_vs_packetLossRatio1.dat" with linespoints title "TcpNewReno" lc rgb 'blue', \
#          "GeneratedFiles/$folder_name/throughput_vs_packetLossRatio2.dat" with linespoints title "TcpWestwoodPlus" lc rgb 'red'
# EOFMarker
# ##+++++++++++++++++ fairness index plot+++++++++++++++++++++++++++++++++
# gnuplot -persist <<-EOFMarker
#     set terminal png size 640,480
#     set output "GeneratedFiles/$folder_name/FairnessIndex_vs_PacketLossRate.png"
#     set xlabel "PacketLossRate"
#     set ylabel "Fairness index"
#     plot "GeneratedFiles/$folder_name/FairnessIndex_vs_PacketLossRate.dat" with linespoints title "TcpNewReno+$folder_name"
# EOFMarker

