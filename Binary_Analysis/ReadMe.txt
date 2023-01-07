==============================================================================================================
				BINARY ANALYSIS TOOL TO EXTRACT SYSTEM CALL GRAPH FROM A STATICALLY LINKED PROGRAM
==============================================================================================================
================						DEVELOPED BY ABHISHEK GHOSH					==========================
================						M.TECH RESEARCH (SYSTEMS POOL)				==========================
================						SR NO: 21091								==========================
==============================================================================================================

The binary analysis tool is a python script name "GenGraphDotFile.py".
The file takes a statically compiled binary, analyzes it and generates the system call graph in dot format.

The requirements for this python script is present in the "requirement.txt" file.

The "GenGraphDotFile.py" is dependent on two more files:

	1. An executable named "database" which is an executable compiled for Intel-x86_64 systems
	2. A text file named "libc.txt"

These two files mentioned above should be placed in the same directory as the python script or else the python 
script shall not work

The python script takes exactly one command line argument.

	1. path or name of the binary which you want to analyze.
	
And saves the .dot file in the same directory as that of the executable with the same name as that of the 
executable, but with a dot extension.

For example, if we have program named as "Complex.c" and assuming that we in our example have kept our script
along with its dependent files in a folder named Binary_Analysis under home directory then we have:

----------------------------------------------------------------------------------------------------------------
$gcc -g -static Complex.c -o Complex
$python3 Binary_Analysis/GenGraphDotFile.py Complex

....
....

$ls
Binary_Analysis Complex.c Complex Complex.dot
$
-----------------------------------------------------------------------------------------------------------------

To render the dot file generated to a pdf to visualize the graph, the python script "render_to_pdf.py" could be 
used. It take exactly one argument. The path to the file containing the dot graph.

$python3 Binary_Analysis/render_to_pdf.py Complex.dot
