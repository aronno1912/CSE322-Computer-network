#!/bin/bash

# # Define an array with different values of nNodes
declare -a nNodesArray=("20" "40" "60" "80" "100")

# Loop over each value of nNodes and run the simulation
for nNodes in "${nNodesArray[@]}"
do
    #g++ -std=c++11 -Wall -o simulation scratch/1905053_1.cc -lns3.39-core-debug -lns3.39-csma-debug -lns3.39-network-debug -lns3.39-internet-debug -lns3.39-mobility-debug -lns3.39-point-to-point-debug -lns3.39-wifi-debug -lns3.39-applications-debug

    # Run the simulation with the current value of nNodes
    ./ns3 run scratch/1905053_1 -- --nNodes=$nNodes --nFlows=50 --nPacketsPerSecond=500 --coverageArea=5 --datFileTrack=1

    # Clean up generated files (optional, comment this line if you want to keep output files)
    rm -rf scratch/*.dat scratch/*.pcap

    # Optionally, you can move or save the output files for each run in separate directories
    # mkdir -p output/nNodes_$nNodes
    # mv scratch/* output/nNodes_$nNodes/

    # Wait for a few seconds between runs (optional, you can adjust the sleep duration as needed)
    sleep 5
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_nNodes.png"
    set xlabel "nNodes"
    set ylabel "Throughput (Mbit/s)"
    plot "throughput_vs_nNodes.dat" with linespoints title "Throughput vs. nNodes"
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "deliveryRatio_vs_nNodes.png"
    set xlabel "nNodes"
    set ylabel "deliveryRatio "
    plot "deliveryRatio_vs_nNodes.dat" with linespoints title "deliveryRatio vs. nNodes"
EOFMarker

##########################################   Varying nFlows    #####################################################################################
# # Define an array with different values of nFlows
declare -a nFlowsArray=("10" "20" "30" "40" "50")

# Loop over each value of nFlows and run the simulation
for nFlows in "${nFlowsArray[@]}"
do
    #g++ -std=c++11 -Wall -o simulation scratch/1905053_1.cc -lns3.39-core-debug -lns3.39-csma-debug -lns3.39-network-debug -lns3.39-internet-debug -lns3.39-mobility-debug -lns3.39-point-to-point-debug -lns3.39-wifi-debug -lns3.39-applications-debug

    # Run the simulation with the current value of nFlows
    #value of reset others are fixed
    ./ns3 run scratch/1905053_1 -- --nNodes=20 --nFlows=$nFlows --nPacketsPerSecond=100 --coverageArea=5 --datFileTrack=2

    # Clean up generated files
    rm -rf scratch/*.dat scratch/*.pcap

    # Optionally, you can move or save the output files for each run in separate directories
    # mkdir -p output/nFlows_$nFlows
    # mv scratch/* output/nFlows_$nFlows/

    # Wait for a few seconds between runs 
    sleep 5
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_nFlows.png"
    set xlabel "nFlows"
    set ylabel "Throughput (Mbit/s)"
    plot "throughput_vs_nFlows.dat" with linespoints title "Throughput vs. nFlows"
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "deliveryRatio_vs_nFlows.png"
    set xlabel "nFlows"
    set ylabel "deliveryRatio "
    plot "deliveryRatio_vs_nFlows.dat" with linespoints title "deliveryRatio vs. nFlows"
EOFMarker
########################################### Varying Coverage area #####################################################################


# # Define an array with different values of coverageArea
declare -a nCoverageArray=("1" "2" "3" "4" "5")

# Loop over each value of coverageArea and run the simulation
for coverageArea in "${nCoverageArray[@]}"
do
    #g++ -std=c++11 -Wall -o simulation scratch/1905053_1.cc -lns3.39-core-debug -lns3.39-csma-debug -lns3.39-network-debug -lns3.39-internet-debug -lns3.39-mobility-debug -lns3.39-point-to-point-debug -lns3.39-wifi-debug -lns3.39-applications-debug

    # Run the simulation with the current value of coverageArea
    #value of reset others are fixed
    ./ns3 run scratch/1905053_1 -- --nNodes=20 --nFlows=10 --nPacketsPerSecond=500 --coverageArea=$coverageArea --datFileTrack=4

    # Clean up generated files
    rm -rf scratch/*.dat scratch/*.pcap

    # Optionally, you can move or save the output files for each run in separate directories
    # mkdir -p output/coverageArea_$coverageArea
    # mv scratch/* output/coverageArea_$coverageArea/

    # Wait for a few seconds between runs 
    sleep 5
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_coverageArea.png"
    set xlabel "coverageArea"
    set ylabel "Throughput (Mbit/s)"
    plot "throughput_vs_coverageArea.dat" with linespoints title "Throughput vs. coverageArea"
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "deliveryRatio_vs_coverageArea.png"
    set xlabel "coverageArea"
    set ylabel "deliveryRatio "
    plot "deliveryRatio_vs_coverageArea.dat" with linespoints title "deliveryRatio vs. coverageArea"
EOFMarker

########################################## Varying nPackets ########################################################################################

# # Define an array with different values of nPacketsPerSecond
declare -a nPacketsArray=("100" "200" "300" "400" "500")

# Loop over each value of nPacketsPerSecond and run the simulation
for nPacketsPerSecond in "${nPacketsArray[@]}"
do
    #g++ -std=c++11 -Wall -o simulation scratch/1905053_1.cc -lns3.39-core-debug -lns3.39-csma-debug -lns3.39-network-debug -lns3.39-internet-debug -lns3.39-mobility-debug -lns3.39-point-to-point-debug -lns3.39-wifi-debug -lns3.39-applications-debug

    # Run the simulation with the current value of nPacketsPerSecond
    #value of reset others are fixed
    ./ns3 run scratch/1905053_1 -- --nNodes=20 --nFlows=10 --nPacketsPerSecond=$nPacketsPerSecond --coverageArea=1 --datFileTrack=3

    # Clean up generated files
    rm -rf scratch/*.dat scratch/*.pcap

    # Optionally, you can move or save the output files for each run in separate directories
    # mkdir -p output/nPacketsPerSecond_$nPacketsPerSecond
    # mv scratch/* output/nPacketsPerSecond_$nPacketsPerSecond/

    # Wait for a few seconds between runs 
    sleep 5
done


#Plot the graph using Gnuplot
gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "throughput_vs_nPacketsPerSecond.png"
    set xlabel "nPacketsPerSecond"
    set ylabel "Throughput (Mbit/s)"
    plot "throughput_vs_nPacketsPerSecond.dat" with linespoints title "Throughput vs. nPacketsPerSecond"
EOFMarker

gnuplot -persist <<-EOFMarker
    set terminal png size 640,480
    set output "deliveryRatio_vs_nPacketsPerSecond.png"
    set xlabel "nPacketsPerSecond"
    set ylabel "deliveryRatio "
    plot "deliveryRatio_vs_nPacketsPerSecond.dat" with linespoints title "deliveryRatio vs. nPacketsPerSecond"
EOFMarker