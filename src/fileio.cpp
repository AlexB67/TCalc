#include <glibmm/i18n.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <tuple>
#include "fileio.hpp"
#include "appglobals.hpp"

void fileIO::get_app_data()
{
   auto listdirs = Glib::get_system_data_dirs();

   for(auto &i: listdirs)
   {
      auto path = i + "gnome-tcalc";
      if (true == std::filesystem::exists(path.c_str()))
      {
		 auto checkforfile = path + "/eyepieces.TCalc";
         if (true == std::filesystem::exists(checkforfile.c_str())) 
		 {
			 AppGlobals::datadir = path;
			 break;
		 }
      }
   }

   // folder used for writing user telescopes and eyepieces data + the configuration file tcalc.conf

   if (true == std::filesystem::exists(AppGlobals::userconfigdir.c_str())) return;

   Glib::RefPtr<Gio::File> file;
   file = Gio::File::create_for_path(AppGlobals::userconfigdir);
   file->make_directory();
}

void fileIO::dbfileIO::load_scope_data(	Gtk::ComboBox& scopecombobox, 
										ScopeCombo::ScopeCombomodel &scopecombomodel,
										const bool userdataonly)
{   
	Glib::ustring path = AppGlobals::telescopesfilename;
	Glib::ustring pathuser = AppGlobals::telescopesfilenameuser;
    
    if (false == std::filesystem::exists(path.c_str())) 
		return;
	
	if (true == std::filesystem::exists(path.c_str()))
		scopecombomodel.create_scope_model_combo(scopecombobox);

	read_scope_file(scopecombobox, pathuser, scopecombomodel);

	if (false == userdataonly) 
		read_scope_file(scopecombobox, path, scopecombomodel);

	scopecombomodel.setup_scope_combo();
}

void fileIO::dbfileIO::load_ep_data(Gtk::ComboBox& epcombobox, 
									EpCombo::EpCombomodel &epcombomodel, 
									const bool userdataonly)
{   
	Glib::ustring path = AppGlobals::eyepiecesfilename;
	Glib::ustring pathuser = AppGlobals::eyepiecesfilenameuser;

	if (false == std::filesystem::exists(path.c_str())) 
		return;

 	if (true == std::filesystem::exists(path.c_str()))
		epcombomodel.create_ep_model_combo(epcombobox);

	read_ep_file(epcombobox, pathuser, epcombomodel);

	if (false == userdataonly) 
		read_ep_file(epcombobox, path, epcombomodel);

	epcombomodel.setup_ep_combo();
}

void fileIO::dbfileIO::load_dso_data(Gtk::ComboBox &dsocombobox, DSOCombomodel &dsocombomodel) const
{
	const Glib::ustring path = AppGlobals::dsolistfilename;

	if (false == std::filesystem::exists(path.c_str()))
		return;

	Glib::ustring dso_name;
	Glib::ustring dso_imagefile;
	double dso_mag;
	double dso_minsize;
	double dso_maxsize;
	double dso_imagesize;
	Glib::ustring dso_type;

	std::ifstream file(path.c_str(), std::ifstream::in);
	std::string line, tmp;

	dsocombomodel.create_dso_model_combo(dsocombobox);

	int linecount = 0;

	while (file.good())
	{
		getline(file, line);
		std::stringstream tokens(line);

		getline(tokens, tmp, '|');

		if (false == tmp.empty())
		{
			if ('#' == tmp.c_str()[0])
				continue; // comment line

			dso_name = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			dso_imagefile = AppGlobals::datadir + '/' + tmp;
			getline(tokens, tmp, '|');
			dso_mag = std::stod(tmp);
			getline(tokens, tmp, '|');
			dso_minsize = std::stod(tmp);
			getline(tokens, tmp, '|');
			dso_maxsize = std::stod(tmp);
			getline(tokens, tmp, '|');
			dso_imagesize = std::stod(tmp);
			getline(tokens, tmp, '|');
			dso_type = static_cast<Glib::ustring>(tmp);

			dsocombomodel.append_dso_to_model(linecount, dso_name, dso_imagefile, dso_mag, dso_minsize, dso_maxsize, dso_imagesize, dso_type);
			++linecount;
		}
	}

	dsocombomodel.setup_dso_combo();

	file.close();
}

void fileIO::dbfileIO::read_scope_file( const Gtk::ComboBox& scopecombobox, 
                            			const Glib::ustring &path, 
                            			const ScopeCombo::ScopeCombomodel &scopecombomodel)
{
	std::ifstream file(path.c_str(), std::ifstream::in);

    std::tuple<	Glib::ustring, double, double, double, double, int, 
            	Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
            	double, double> scopedata;

    std::string line, tmp;
	unsigned int size = scopecombobox.get_model()->children().size();

	if ( size)
	{
		scopedata = {"", 0.0, 0.0, 0.0 , 0.0, 100, "", "", "", "", 0.0, 0.0}; // separator = 100
		scopecombomodel.append_scope_to_model(scopedata);
	}
    
    while (file.good())
	{
	    getline(file, line);
	    std::stringstream tokens(line);
	    
	    getline(tokens, tmp, '|');
	    
	    if(false == tmp.empty()) // skip blanc lines
	    {   
			if('#' == tmp.c_str()[0])
				continue;  // comment line
			
			std::get<0>(scopedata) = static_cast<Glib::ustring>(tmp);
	        getline(tokens, tmp, '|');
			std::get<1>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<2>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<3>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<4>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<5>(scopedata) = std::stoi(tmp);
			getline(tokens, tmp, '|');
			std::get<6>(scopedata) = static_cast<Glib::ustring>(tmp);
	        getline(tokens, tmp, '|');
			std::get<7>(scopedata) = static_cast<Glib::ustring>(tmp);
	        getline(tokens, tmp, '|');
			std::get<8>(scopedata) = static_cast<Glib::ustring>(tmp);
	        getline(tokens, tmp, '|');
			std::get<9>(scopedata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<10>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<11>(scopedata) = std::stod(tmp);
	    
	        scopecombomodel.append_scope_to_model(scopedata);
	    } 	       
	}
   
    file.close();
}

void fileIO::dbfileIO::read_ep_file(const Gtk::ComboBox& epcombobox, 
									const Glib::ustring &path, 
									const EpCombo::EpCombomodel &epcombomodel)
{
	std::ifstream file(path.c_str(), std::ifstream::in);

	std::tuple<Glib::ustring, double, double, double, double, double, double, 
			   Glib::ustring, int, int, double, Glib::ustring, Glib::ustring> epdata;

	std::string line, tmp;
	unsigned int size = epcombobox.get_model()->children().size();

	if ( size )
	{
		epdata= {	
					"", 0.0, 0.0, 0.0 , 0.0, 0.0, 0.0, "separator", 
					0, 0, 0.0, "", ""
				}; // type reused as separator
		epcombomodel.append_ep_to_model(epdata);
	}

	while (file.good())
	{

		getline(file, line);
		std::stringstream tokens(line);

		getline(tokens, tmp, '|');

		if (false == tmp.empty()) // skip blanc lines
		{
			if ('#' == tmp.c_str()[0])
				continue; // comment line

			std::get<0>(epdata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<1>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<2>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<3>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<4>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<5>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<6>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<7>(epdata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<8>(epdata) = std::stoi(tmp);
			getline(tokens, tmp, '|');
			std::get<9>(epdata) = std::stoi(tmp);
			getline(tokens, tmp, '|');
			std::get<10>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<11>(epdata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<12>(epdata) = static_cast<Glib::ustring>(tmp);

			epcombomodel.append_ep_to_model(epdata);
		}
	}

	file.close();
};


void fileIO::dbfileIO::write_ep_user_data(	const Gtk::ComboBox& epcombobox, 
											const  EpCombo::EpCombomodel &epcombomodel) const
{
		Glib::ustring pathuser = AppGlobals::eyepiecesfilenameuser;
        std::ofstream outfile;
        outfile.open(pathuser.c_str(), std::ios_base::trunc);
        char sep = '|';

        for (auto& iter : epcombobox.get_model()->children())
        {
            outfile << iter->get_value(epcombomodel.m_epcols.m_epmodel)  << sep
                    << iter->get_value(epcombomodel.m_epcols.m_epfov)    << sep
                    << iter->get_value(epcombomodel.m_epcols.m_epflen)   << sep
                    << iter->get_value(epcombomodel.m_epcols.m_epfstop)  << sep
                    << iter->get_value(epcombomodel.m_epcols.m_eprelief) << sep
                    << iter->get_value(epcombomodel.m_epcols.m_eptrans)  << sep
                    << iter->get_value(epcombomodel.m_epcols.m_epbarrel) << sep
                    << iter->get_value(epcombomodel.m_epcols.m_eptype)   << sep
					<< iter->get_value(epcombomodel.m_epcols.m_epgroups) << sep
					<< iter->get_value(epcombomodel.m_epcols.m_epelements) << sep
					<< iter->get_value(epcombomodel.m_epcols.m_epweight)   << sep
					<< iter->get_value(epcombomodel.m_epcols.m_epcoating)  << sep
					<< iter->get_value(epcombomodel.m_epcols.m_epmaterial) << sep << '\n';
        }

        outfile.close();
}

void fileIO::dbfileIO::write_scope_user_data(const Gtk::ComboBox& scopecombobox,
											const  ScopeCombo::ScopeCombomodel &scopecombomodel) const
{
		Glib::ustring pathuser = AppGlobals::telescopesfilenameuser;
        std::ofstream outfile;
        outfile.open(pathuser.c_str(), std::ios_base::trunc);
        char sep = '|';

        for (auto& iter : scopecombobox.get_model()->children())
        {
            outfile << iter->get_value(scopecombomodel.m_scopecols.m_smodel)  	<< sep
                    << iter->get_value(scopecombomodel.m_scopecols.m_saperture)	<< sep
                    << iter->get_value(scopecombomodel.m_scopecols.m_sflen)   	<< sep
                    << iter->get_value(scopecombomodel.m_scopecols.m_sobstruct)	<< sep
                    << iter->get_value(scopecombomodel.m_scopecols.m_sreflect) 	<< sep
                    << iter->get_value(scopecombomodel.m_scopecols.m_stype)   	<< sep
					<< iter->get_value(scopecombomodel.m_scopecols.m_smirrorcoating)  << sep
    				<< iter->get_value(scopecombomodel.m_scopecols.m_smirrormaterial) << sep
    				<< iter->get_value(scopecombomodel.m_scopecols.m_slenscoating)  << sep
    				<< iter->get_value(scopecombomodel.m_scopecols.m_slensmaterial) << sep
    				<< iter->get_value(scopecombomodel.m_scopecols.m_sstrehl) << sep
    				<< iter->get_value(scopecombomodel.m_scopecols.m_sweight) << sep << '\n';
        }

        outfile.close();
}
