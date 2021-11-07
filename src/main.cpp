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
// app->add_action("quit", sigc::ptr_fun<void>(&close));
int main (int argc, char *argv[])
{
	auto app = Gtk::Application::create("org.gnome.TCalc");
	return app->make_window_and_run<TcalcWindow>(argc, argv, app);
}