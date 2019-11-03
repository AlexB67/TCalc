#include "gtkmmcustomutils.hpp"
#include <glibmm/i18n.h>

void Ui::SpinEntry::set_spin_entry(double value, double min, double max, double incr, int digits, bool isnumeric)
{
    set_range (min, max);
	set_value(value);
	set_increments(incr, 10.0 * incr);
	set_digits (digits);
	set_numeric(isnumeric);
}
