#!/usr/bin/env python3

from os import environ, makedirs, path
from pathlib import Path
from shutil import copy
from subprocess import call

prefix = environ.get('MESON_INSTALL_PREFIX', '/usr/local')
datadir = path.join(prefix, 'share')
destdir = environ.get('DESTDIR', '')

# Package managers set this so we don't need to run
if not destdir:
    print('Updating icon cache...')
    call(['gtk-update-icon-cache', '-qtf', path.join(datadir, 'icons', 'hicolor')])

    print('Updating desktop database...')
    call(['update-desktop-database', '-q', path.join(datadir, 'applications')])

    print('Compiling GSettings schemas...')
    call(['glib-compile-schemas', path.join(datadir, 'glib-2.0', 'schemas')])

print('Installing user data ...')
srcdir=('../../appdata')
destdir=(str(Path.home()) + '/Documents/TCalc')

if not os.path.exists(destdir): 
    os.mkdir(destdir)

for filename in os.listdir(srcdir):
    shutil.copy( srcdir + filename, destdir)
