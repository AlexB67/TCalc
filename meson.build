project('tcalc', ['cpp', 'c'], 
        version: '0.0.8',
  meson_version: '>= 0.47.0',
  default_options : ['buildtype=release']
)

i18n = import('i18n')
gnome = import('gnome')
pkgconfig = import('pkgconfig')

subdir('data')
subdir('help')
subdir('src')
subdir('po')


meson.add_install_script('build-aux/meson/postinstall.py')

