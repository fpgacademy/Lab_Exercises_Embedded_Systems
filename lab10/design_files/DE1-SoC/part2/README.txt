To reprogram the FPGA, first ensure that you do not have any kernel modules
inserted (lsmod). If so, remove them (rmmod "kernel_name"). Then you can use the command:

~/misc/program_fpga ./Edge_Detector_System.rbf

If successful, this command should print something like:

2+1 records in
2+1 records out
XXX bytes (XXX MB) copied, ...

