#!/usr/bin/env python3

import os
import shutil
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


print('Installing application data ...')

appdatadest = datadir + '/gnome-tcalc/'

if not os.path.exists(appdatadest):
    os.mkdir(appdatadest)

os.chdir('../appdata/')
appsrcdir=(os.curdir)

for filename in os.listdir(appsrcdir):
    shutil.copy(filename, appdatadest)
