desktop_file = i18n.merge_file(
  input: 'org.gnome.TCalc.desktop.in',
  output: 'org.gnome.TCalc.desktop',
  type: 'desktop',
  po_dir: '../po',
  install: true,
  install_dir: join_paths(get_option('datadir'), 'applications')
)

desktop_utils = find_program('desktop-file-validate', required: false)
if desktop_utils.found()
  test('Validate desktop file', desktop_utils,
    args: [desktop_file]
  )
endif

appstream_file = i18n.merge_file(
  input: 'org.gnome.TCalc.appdata.xml.in',
  output: 'org.gnome.TCalc.appdata.xml',
  po_dir: '../po',
  install: true,
  install_dir: join_paths(get_option('datadir'), 'appdata')
)

appstream_util = find_program('appstream-util', required: false)
if appstream_util.found()
  test('Validate appstream file', appstream_util,
    args: ['validate', appstream_file]
  )
endif

install_data('org.gnome.TCalc.gschema.xml',
  install_dir: join_paths(get_option('datadir'), 'glib-2.0/schemas')
) 

compile_schemas = find_program('glib-compile-schemas', required: false)
if compile_schemas.found()
  test('Validate schema file', compile_schemas,
    args: ['--strict', '--dry-run', meson.current_source_dir()]
  )
endif

# more icons to create, for now just use one

foreach s: ['48', '256', '512']
  icon_destdir = join_paths(get_option('datadir'), 'icons', 'hicolor', '@0@x@0@'.format(s), 'apps')
  install_data('tcalc.png', install_dir: icon_destdir)
endforeach

# essential files needed by tcalc 

install_data('tcalcdata/eyepieces.TCalc',   install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))
install_data('tcalcdata/telescopes.TCalc',  install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))
install_data('tcalcdata/skyobjects.TCalc',  install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))
install_data('tcalcdata/m13.png',           install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))
install_data('tcalcdata/m51.png',           install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))
install_data('tcalcdata/m57.png',           install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))
install_data('tcalcdata/m101.png',          install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))
install_data('tcalcdata/NGC6116.png',       install_dir: join_paths(get_option('datadir'), 'gnome-tcalc'))











