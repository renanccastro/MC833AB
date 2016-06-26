#!/usr/bin/perl

$numRounds = 10000000; # number of rounds to perform per simulation
$dim = 1000;
$r = 100;
$PI=3.14152;

for($numNodes=512; $numNodes<=8192; $numNodes*=2) {
	for($density=10; $density<=30; $density+=5) {
		for ($i=0; $i<5; $i++) { 
			print "Numero de nós:  $numNodes\n";
			print "Densidade:  $density\n";
			$dim = int(sqrt($numNodes*$PI*$r*$r/$density));
			system("java -cp binaries/bin sinalgo.Run " .
					"-project PIF " .             # choose the project
					"-gen $numNodes PIF:PIFNode Random " . # generate nodes
					"-batch " .
					"-overwrite " .                   # Overwrite configuration file parameters
#"exitAfter=true exitAfter/Rounds=$numRounds " . # number of rounds to perform & stop
#"exitOnTerminationInGUI=true " .  # Close GUI when hasTerminated() returns true
					"AutoStart=true " .               # Automatically start communication protocol
#"outputToConsole=false " .        # Create a framework log-file for each run
#"extendedControl=false " .        # Don't show the extended control in the GUI
					"mobility=false " .
					"dimX=$dim " .
					"dimY=$dim " .
					"udg/rmax=$r " .
					"-rounds $numRounds " .           # Number of rounds to start simulation
					"-refreshRate 20");               # Don't draw GUI often
		}
	} 
}
