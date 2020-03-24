# Circuit Logic Simulator #
### Build Instructions ###
1.	**Install dependencies**
	-	Qt base: `qt5-base`
	-	QMake: `qt5-tools`
	-	Compiler: `g++` or `clang`, must support C++ 11 or later
	-	Note that your distro's package names  might vary. All provided package names are those used by Archlinux.
2.	**Clone the repository and cd into it**

	$`git clone https://github.com/xypwn/circuit-logic-simulator.git`
	
	$`cd circuit-logic-simulator`
3.	**Create a build directory and cd into it**

	$`mkdir build`
	
	$`cd build`
4.	**Configure build using QMake**

	$`qmake ..`
5.	**Start build (replace 4 with the number of threads you have to speed it up)**

	$`make -j 4`
6.	**Run the executable**

	There should now be an executable file called `CircuitSimulator` in your
	build folder. You can put it anywhere or run it with:
	
	$`./CircuitSimulator`
### Notes ###
-	The software is currently under heavy development and therefore still crawling with bugs
-	The `About` and `Tutorial` menu item haven't yet been implemented
-	Although some icons are missing, you can hover over an item to see what it does