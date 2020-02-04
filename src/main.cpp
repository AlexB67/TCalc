/* main.cpp
 *
 * Copyright 2019 Alexander Borro
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GPL v3.0
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <glibmm/i18n.h>
#include "tcalcwindow.hpp"
#include "appglobals.hpp"
#include <iostream>

Gtk::Window  *tcalcwindow;

static void on_activate (Glib::RefPtr<Gtk::Application>& tcalcapp)
{
	tcalcwindow = tcalcapp->get_active_window();

	if (!tcalcwindow)
	{
			tcalcwindow = new TcalcWindow(tcalcapp); 
			tcalcwindow->property_application() = tcalcapp;
			tcalcapp->add_window(*tcalcwindow);
	}

	tcalcwindow->present();
}

int main (int argc, char *argv[])
{
	argc = 1; // command line args not supported yet.
	int returncode;

	auto tcalcapp = Gtk::Application::create("org.gnome.TCalc", Gio::APPLICATION_FLAGS_NONE);
	tcalcapp->signal_activate().connect(sigc::bind(&on_activate, tcalcapp));
	returncode = tcalcapp->run(argc, argv);

	delete tcalcwindow;

	if (0 == returncode)
		AppGlobals::app_notify(_("Clear skies!"), tcalcapp);
	else
		AppGlobals::app_notify(_("TCalc exited abnormally. Exit code : ") + 
								Glib::ustring::format(returncode), tcalcapp);
	
	return returncode;
}