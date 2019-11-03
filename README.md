<<<<<<< HEAD
TCalc - An astronomy tool for telescopes and eyepieces.

Warnig: very much work in progress, hence the cumbersome installation steps

clone the repo

build gtkmm-plplot from https://github.com/tschoonj/gtkmm-plplot


Copy the contents of Documents as follows

create a folder TCalc in /home/yourusername/Documents/TCalc

copy contents of the Documents in the repo folder to /home/yourusername/Documents/TCalc

From the command line 
$cd TCalc
$mkdir build
$meson build/
$cd build
$ninja

you can now run the app from the build folder with the command /src/tcalc

optionally 

$ninja install

This will add the application to user/local and appear in the applications list.

Note: this is a vscode project but works equaly well in gnome-builder, just point builder to the TCalc folder and open it

to be build as flatpak or with rpm in future.



=======
# TCalc
An astronomy tool for telescopes and eyepieces.
Instructions to add here.
>>>>>>> f2e031981a5080c5a2a83d753412b0224291416a
