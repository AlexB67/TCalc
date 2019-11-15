
# essential files needed by tcalc 

tcalcdata = [
    'eyepieces.TCalc', 
    'telescopes.TCalc', 
    'skyobjects.TCalc', 
    'm13.png', 
    'm51.png', 
    'm57.png', 
    'm101.png', 
    'NGC6116.png',   
    'm82.png'
]

foreach file : tcalcdata
    install_data(file,   install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))
endforeach

#HACK so they are also copied to build tree

foreach file : tcalcdata
    configure_file(input: file, output: file, copy: true)
endforeach

