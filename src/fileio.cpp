#include <glibmm/i18n.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <tuple>
#include "fileio.hpp"
#include "appglobals.hpp"

void fileIO::set_app_data()
{
	if(false == std::filesystem::exists(AppGlobals::eyepiecesfilename.c_str())) // if running tcalc uninstalled
	{																
		auto path = AppGlobals::builddir + "/data/tcalcdata/";
		AppGlobals::eyepiecesfilename = path + "eyepieces.TCalc";
		AppGlobals::telescopesfilename = path + "telescopes.TCalc";
		AppGlobals::dsolistfilename = path + "skyobjects.TCalc";
		AppGlobals::datadir = path;
	}

	// folder used for writing user telescopes and eyepieces data + the configuration file tcalc.conf

	if (true == std::filesystem::exists(AppGlobals::userconfigdir.c_str())) return;

	Glib::RefPtr<Gio::File> file;
	file = Gio::File::create_for_path(AppGlobals::userconfigdir);
	file->make_directory();
}

void fileIO::dbfileIO::load_scope_data(ScopeCombo::ScopeCombomodel &scopecombomodel, const bool userdataonly)
{   
	Glib::ustring path = AppGlobals::telescopesfilename;
	Glib::ustring pathuser = AppGlobals::telescopesfilenameuser;
    
    if (false == std::filesystem::exists(path.c_str())) 
		return;
	
	if (true == std::filesystem::exists(path.c_str()))
		scopecombomodel.create_scope_model();

	read_scope_file(pathuser, scopecombomodel);

	if (false == userdataonly) 
		read_scope_file(path, scopecombomodel);
}

void fileIO::dbfileIO::load_ep_data(EpCombo::EpCombomodel &epcombomodel, const bool userdataonly)
{   
	Glib::ustring path = AppGlobals::eyepiecesfilename;
	Glib::ustring pathuser = AppGlobals::eyepiecesfilenameuser;

	if (false == std::filesystem::exists(path.c_str())) 
		return;

 	if (true == std::filesystem::exists(path.c_str()))
		epcombomodel.create_ep_model();

	read_ep_file(pathuser, epcombomodel);

	if (false == userdataonly) 
		read_ep_file(path, epcombomodel);
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

void fileIO::dbfileIO::read_scope_file(const Glib::ustring &path, ScopeCombo::ScopeCombomodel &scopecombomodel)
{
	std::ifstream file(path.c_str(), std::ifstream::in);

    std::tuple<	Glib::ustring, Glib::ustring, double, double, double, double, int, 
            	Glib::ustring, Glib::ustring, Glib::ustring, Glib::ustring, 
            	double, double, double, Glib::ustring, Glib::ustring, Glib::ustring > scopedata;

    std::string line, tmp;
	
    while (file.good())
	{
	    getline(file, line);
	    std::stringstream tokens(line);
	    
	    getline(tokens, tmp, '|');
	    
	    if(false == tmp.empty()) // skip blanc lines
	    {   
			if('#' == tmp.c_str()[0])
				continue;  // comment line
			
			if (current_brand != static_cast<Glib::ustring>(tmp))
			{
				current_brand = static_cast<Glib::ustring>(tmp);
				std::get<0>(scopedata) = static_cast<Glib::ustring>(tmp);
				scopecombomodel.append_scope_to_model(scopedata); // parent
			}
			std::get<0>(scopedata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<1>(scopedata) = static_cast<Glib::ustring>(tmp);
	        getline(tokens, tmp, '|');
			std::get<2>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<3>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<4>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<5>(scopedata) = std::stod(tmp);
	        getline(tokens, tmp, '|');
			std::get<6>(scopedata) = std::stoi(tmp);
			getline(tokens, tmp, '|');
			std::get<7>(scopedata) = static_cast<Glib::ustring>(tmp);
	        getline(tokens, tmp, '|');
			std::get<8>(scopedata) = static_cast<Glib::ustring>(tmp);
	        getline(tokens, tmp, '|');
			std::get<9>(scopedata) = static_cast<Glib::ustring>(tmp);
	        getline(tokens, tmp, '|');
			std::get<10>(scopedata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<11>(scopedata) = std::stod(tmp); // strehl
	        getline(tokens, tmp, '|');
			std::get<12>(scopedata) = std::stod(tmp); // weight
			getline(tokens, tmp, '|');
			std::get<13>(scopedata) = std::stod(tmp); // mount weight
			getline(tokens, tmp, '|');
			std::get<14>(scopedata) = static_cast<Glib::ustring>(tmp); // mount type
			getline(tokens, tmp, '|');
			std::get<15>(scopedata) = static_cast<Glib::ustring>(tmp); // focuser type
			getline(tokens, tmp, '|');
			std::get<16>(scopedata) = static_cast<Glib::ustring>(tmp); // finder type
	        scopecombomodel.append_scope_to_model(scopedata, true);  // child
	    } 	       
	}
   
    file.close();
}

void fileIO::dbfileIO::read_ep_file(const Glib::ustring &path, EpCombo::EpCombomodel &epcombomodel)
{
	std::ifstream file(path.c_str(), std::ifstream::in);

	std::tuple<Glib::ustring, Glib::ustring, double, double, double, double, double, double, 
			   Glib::ustring, int, int, double, Glib::ustring, Glib::ustring> epdata;

	std::string line, tmp;

	while (file.good())
	{

		getline(file, line);
		std::stringstream tokens(line);

		getline(tokens, tmp, '|');

		if (false == tmp.empty()) // skip blanc lines
		{
			if ('#' == tmp.c_str()[0])
				continue; // comment line

			if (current_brand != static_cast<Glib::ustring>(tmp))
			{
				current_brand = static_cast<Glib::ustring>(tmp);
				std::get<0>(epdata) = static_cast<Glib::ustring>(tmp);
				epcombomodel.append_ep_to_model(epdata); // parent
			}
			std::get<0>(epdata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<1>(epdata) = static_cast<Glib::ustring>(tmp);
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
			std::get<7>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<8>(epdata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<9>(epdata) = std::stoi(tmp);
			getline(tokens, tmp, '|');
			std::get<10>(epdata) = std::stoi(tmp);
			getline(tokens, tmp, '|');
			std::get<11>(epdata) = std::stod(tmp);
			getline(tokens, tmp, '|');
			std::get<12>(epdata) = static_cast<Glib::ustring>(tmp);
			getline(tokens, tmp, '|');
			std::get<13>(epdata) = static_cast<Glib::ustring>(tmp);
			epcombomodel.append_ep_to_model(epdata, true); // child
		}
	}

	current_brand.clear();

	file.close();
}


void fileIO::dbfileIO::write_ep_user_data(	const Gtk::ComboBox& epcombobox, 
											const  EpCombo::EpCombomodel &epcombomodel) const
{
		Glib::ustring pathuser = AppGlobals::eyepiecesfilenameuser;
        std::ofstream outfile;
        outfile.open(pathuser.c_str(), std::ios_base::trunc);
        char sep = '|';

        for (auto& iter : epcombobox.get_model()->children())
        {
			for(auto &iter2 : iter.children())
            outfile << iter.get_value(epcombomodel.m_epcols.m_epbrand)  << sep
					<< iter2.get_value(epcombomodel.m_epcols.m_epmodel)  << sep
                    << iter2.get_value(epcombomodel.m_epcols.m_epfov)    << sep
                    << iter2.get_value(epcombomodel.m_epcols.m_epflen)   << sep
                    << iter2.get_value(epcombomodel.m_epcols.m_epfstop)  << sep
                    << iter2.get_value(epcombomodel.m_epcols.m_eprelief) << sep
                    << iter2.get_value(epcombomodel.m_epcols.m_eptrans)  << sep
                    << iter2.get_value(epcombomodel.m_epcols.m_epbarrel) << sep
                    << iter2.get_value(epcombomodel.m_epcols.m_eptype)   << sep
					<< iter2.get_value(epcombomodel.m_epcols.m_epgroups) << sep
					<< iter2.get_value(epcombomodel.m_epcols.m_epelements) << sep
					<< iter2.get_value(epcombomodel.m_epcols.m_epweight)   << sep
					<< iter2.get_value(epcombomodel.m_epcols.m_epcoating)  << sep
					<< iter2.get_value(epcombomodel.m_epcols.m_epmaterial) << sep << '\n';
        }

        outfile.close();
		if (0 == epcombobox.get_model()->children().size()) std::remove(pathuser.c_str());
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
			for(auto &iter2 : iter.children())
            outfile << iter.get_value(scopecombomodel.m_scopecols.m_sbrand)  	<< sep
					<< iter2.get_value(scopecombomodel.m_scopecols.m_smodel)  	<< sep
                    << iter2.get_value(scopecombomodel.m_scopecols.m_saperture)	<< sep
                    << iter2.get_value(scopecombomodel.m_scopecols.m_sflen)   	<< sep
                    << iter2.get_value(scopecombomodel.m_scopecols.m_sobstruct)	<< sep
                    << iter2.get_value(scopecombomodel.m_scopecols.m_sreflect) 	<< sep
                    << iter2.get_value(scopecombomodel.m_scopecols.m_stype)   	<< sep
					<< iter2.get_value(scopecombomodel.m_scopecols.m_smirrorcoating)  << sep
    				<< iter2.get_value(scopecombomodel.m_scopecols.m_smirrormaterial) << sep
    				<< iter2.get_value(scopecombomodel.m_scopecols.m_slenscoating)  << sep
    				<< iter2.get_value(scopecombomodel.m_scopecols.m_slensmaterial) << sep
    				<< iter2.get_value(scopecombomodel.m_scopecols.m_sstrehl) << sep
    				<< iter2.get_value(scopecombomodel.m_scopecols.m_sweight) << sep 
					<< iter2.get_value(scopecombomodel.m_scopecols.m_stube_weight) << sep
					<< iter2.get_value(scopecombomodel.m_scopecols.m_smount_type) << sep
					<< iter2.get_value(scopecombomodel.m_scopecols.m_sfocuser_type) << sep
					<< iter2.get_value(scopecombomodel.m_scopecols.m_sfinder_type) << sep << '\n';
        }

        outfile.close();
}
