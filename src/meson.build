conf_data = configuration_data()
conf_data.set_quoted('PACKAGE_DATADIR', join_paths(get_option('prefix')))
configure_file(
input:  '../tcalcconfig.hpp.meson',
output: 'tcalcconfig.hpp',
configuration: conf_data)

configuration_inc = include_directories('.')

tcalc_sources = [
  'main.cpp',
  'tcalcwindow.cpp',
  'tcalcwindowsignals.cpp',
  'telescopebox.cpp',
  'telescopeboxsignals.cpp',
  'telescopeboxedit.cpp',
  'telescopeboxeditsignals.cpp',
  'eyepiecebox.cpp',
  'eyepieceboxsignals.cpp',
  'eyepieceboxedit.cpp',
  'eyepieceboxeditsignals.cpp',
  'loggerbox.cpp',
  'calcbox.cpp',
  'starmagbox.cpp',
  'starmagboxsignals.cpp',
  'optionsbox.cpp',
  'astrocalc.cpp',
  'gtkmmcustomutils.cpp',
  'telescopecombomodel.cpp',
  'eyepiececombomodel.cpp',
  'fileio.cpp',
  'resultsbox.cpp',
  'resultsboxsignals.cpp',
  'prefswindow.cpp',
  'graphswindow.cpp',
  'graphswindowsignals.cpp',
  'plots.cpp',
  'oculardrawingarea.cpp',
  'ocularwindow.cpp',
  'ocularwindowsignals.cpp',
  'searchwindow.cpp',
  'equipwindow.cpp',
  'helpwindow.cpp',
  'dsocombomodel.cpp'
]

tcalc_deps = [
  dependency('gtkmm-3.0', version: '>= 3.24'),
  dependency('gtkmm-plplot-2.0', version: '>= 2.3'),
  dependency('webkit2gtk-4.0', version: '>= 2.2'),
]

gnome = import('gnome')

tcalc_sources += gnome.compile_resources(
  'tcalc-resources',
  'resources/tcalc.gresource.xml',
  c_name: 'tcalc'
)

executable('tcalc', tcalc_sources,
  dependencies: tcalc_deps, include_directories : configuration_inc,
  override_options : ['cpp_std=c++17'],
  cpp_args : '-Wl, -Wall, -Wextra -Wshadow',
  install: true,
)


