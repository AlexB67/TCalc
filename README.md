GNOME TCalc - An astronomy tool for telescopes and eyepieces written in gtkmm.

Warning: very much work in progress, some things don't work fully as intended

build gtkmm-plplot and install 
from https://github.com/tschoonj/gtkmm-plplotvrsion, version 2.3 or later is required.

clone the Tcalc repo

From the command line 
$cd TCalc
$mkdir build
$meson build/
$cd build
$ninja
$sudo ninja install

This will add the application to /user/local and appear in the applications list as GNOME TCalc.

Note: this is a vscode project but works equally well in gnome-builder, just point builder to the TCalc folder and open it.

Neither are required to build/install the project

To be build as flatpak and/or with rpm in future. 

