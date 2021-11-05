conf_data = configuration_data()
conf_data.set_quoted('PACKAGE_DATADIR', join_paths(get_option('prefix')))
conf_data.set_quoted('TCALC_BUILD_DIR', meson.build_root())
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
  'dsocombomodel.cpp',
  'plotlib/plotter.cpp',
  'plotlib/plotterlabels.cpp',
  'plotlib/plotteroptions.cpp',
  'plotlib/plotterthemes.cpp',
  'plotlib/plotterwidgets.cpp',
  'plotlib/plotterevents.cpp',
  'plotlib/plotterseries.cpp',
  'plotlib/plottercoords.cpp',
  'astrocalclib/astrocalc.cpp',
]

tcalc_deps = [
  dependency('gtkmm-4.0', version: '>= 4.00'),
]

tcalc_sources += gnome.compile_resources(
  'tcalc-resources',
  'resources/tcalc.gresource.xml',
  c_name: 'tcalc'
)

executable('tcalc', tcalc_sources,
  dependencies: tcalc_deps, include_directories : configuration_inc,
  override_options : ['cpp_std=c++17'],
  cpp_args : ['-Wall', '-Wextra', '-pedantic'],
  install: true,
)


