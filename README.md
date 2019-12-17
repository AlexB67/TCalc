GNOME TCalc - An astronomy tool for telescopes and eyepieces written in gtkmm for amateur astronomers.\
Help Documentation is included how to use the application.

Warning: very much work in progress, some things don't work fully as intended
Clone the Tcalc repo, at the comand prompt - 

$git clone https://github.com/AlexB67/TCalc.git

From the command line

$cd TCalc\
$mkdir build\
$meson build/\
$cd build\
$ninja\
$sudo ninja install

To uninstall

$sudo ninja uninstall

This will add the application to /user/local and appear in the applications list as GNOME TCalc.

Note: this is a vscode project but works equally well in gnome-builder, just point builder to the TCalc folder and open it.

Neither are required to build/install the project

To be build as flatpak and/or with rpm in future. 

