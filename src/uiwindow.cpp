#include "uiwindow.h"
#include <string>
#include <stdexcept>

using namespace std;

PlotPhaseGtkWindows* gtkwME;

void PlotPhaseGtkWindows::addPhasor(double magnitude, double angle, char name[255], short r, short g, short b){
	Gtk::TreeModel::Row row = *(ListModel->append());
	row[m_Cols.m_col_name] = name;
	row[m_Cols.m_col_magnitude] = magnitude;
	row[m_Cols.m_col_angle] = angle;
	phaseData thisPhase;
	thisPhase.magnitude = magnitude;
	thisPhase.phase = angle;
	memcpy(&thisPhase.name, &name, 255);
	thisPhase.colR = (char)r;
	thisPhase.colG = (char)g;
	thisPhase.colB = (char)b;
	phases.push_back(thisPhase);
}

static
void on_btnWantAdd_clicked(){
	gtkwME->addDialog->show();
}

int PlotPhaseGtkWindows::RunMainWindow(){
	return app->run(*pWindow);
}

PlotPhaseGtkWindows::PlotPhaseGtkWindows(int argc, char **argv){
	//gtkmm stuff...
	//I don't know why I need this line. (I just know that I do.) If someone knows, add a comment here please!
	//You can tell I'm not a gtk expert.
	app = Gtk::Application::create(argc, argv, "org.djabailey.plotphase");

	bool hasError = false;
	//Load the GtkBuilder file and instantiate its widgets:
	Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
	try
	{
  		refBuilder->add_from_file("./interface.glade");
	}
	catch(const Glib::FileError& ex)
	{
		printf("File error: %s\n", ex.what().data());
		hasError = true;
	}
  	catch(const Glib::MarkupError& ex)
	{
		printf("Markup error: %s\n", ex.what().data());
		hasError = true;
	}
	catch(const Gtk::BuilderError& ex)
	{
		printf("Builder error: %s\n", ex.what().data());
		hasError = true;
	}

	//Get the GtkBuilder-instantiated Window:
	if (!hasError){
		refBuilder->get_widget("window1", pWindow);
		if(pWindow)
  		{
			//refBuilder->get_widget("openDialog", openDialog);

			//Glib::RefPtr<Gtk::FileFilter> stlfilter = Gtk::FileFilter::create();
			//stlfilter->set_name("STL files");
			//stlfilter->add_pattern("*.[Ss][Tt][Ll]");
			//openDialog->add_filter(stlfilter);

			refBuilder->get_widget("entryMag", entryMag);
			refBuilder->get_widget("entryAng", entryAng);
			refBuilder->get_widget("entryName", entryName);

			refBuilder->get_widget("btnAddPhasor", btnAddPhase);
			refBuilder->get_widget("btnDel", btnDelPhase);

			refBuilder->get_widget("btnOpen", btnOpen);
			refBuilder->get_widget("btnSave", btnSave);

			refBuilder->get_widget("tvPhasors", tvPhasors);

			refBuilder->get_widget("btnColourSelect", btnColour);

			refBuilder->get_widget("cbArrow", cbArrow);
			refBuilder->get_widget("cbScope", cbScope);
			refBuilder->get_widget("cbTotal", cbTotal);
			refBuilder->get_widget("cbNames", cbNames);

			refBuilder->get_widget("scaleLine", scaleLine);

			ListModel = Gtk::ListStore::create(m_Cols);

			tvPhasors->set_model(ListModel);
			tvPhasors->append_column("Name", m_Cols.m_col_name);
			tvPhasors->append_column("Magnitude", m_Cols.m_col_magnitude);
			tvPhasors->append_column("Angle", m_Cols.m_col_angle);

			//set up handlers
			gtkwME = this; //pointer to self for use by static handlers

			btnAddPhase->signal_clicked().connect( sigc::ptr_fun(on_btnWantAdd_clicked) );
			addDialog = new PlotPhaseGtkDialogAddPhasor(refBuilder);
  		}
	}
}

PlotPhaseGtkWindows::~PlotPhaseGtkWindows(){
	delete pWindow;
	delete btnAddPhase;
	delete btnDelPhase;
	delete btnOpen;
	delete btnSave;
	delete entryAng;
	delete entryMag;
}

PlotPhaseGtkDialogAddPhasor *gtkwDialogAddME;

static
void on_btnCancel_clicked(){
	gtkwDialogAddME->entryName->set_text("");
	gtkwDialogAddME->entryMag->set_text("");
	gtkwDialogAddME->entryAng->set_text("");
	gtkwDialogAddME->pDialog->hide();
}

static
void on_btnAccept_clicked(){
	Gdk::Color selectedColour = gtkwDialogAddME->btnColour->get_color();
	short r = selectedColour.get_red();
	short g = selectedColour.get_green();
	short b = selectedColour.get_blue();

	double magnitude = atof(gtkwDialogAddME->entryMag->get_text().c_str());
	double angle = atof(gtkwDialogAddME->entryAng->get_text().c_str());
	const char* name_c_str = gtkwDialogAddME->entryName->get_text().c_str();
	if (magnitude>0){
		int nameLen = strlen(name_c_str);
		if (nameLen <= 255){
			char name[255];
			memcpy(name, name_c_str, sizeof(char)*nameLen);
			gtkwME->addPhasor(magnitude, angle, name, r, g, b);
		}
		else{
			printf("Name must be fewer than 255 characters\n");
		}
	}
	else{
		printf("Magnitude must be positive\n");
	}
	on_btnCancel_clicked();
}

PlotPhaseGtkDialogAddPhasor::PlotPhaseGtkDialogAddPhasor(Glib::RefPtr<Gtk::Builder> refBuilder){
	bool hasError = false;

		//Get the GtkBuilder-instantiated Window:
		if (!hasError){
			refBuilder->get_widget("dialogAddPhasor", pDialog);
			if(pDialog)
	  		{
				refBuilder->get_widget("entryMag", entryMag);
				refBuilder->get_widget("entryAng", entryAng);
				refBuilder->get_widget("entryName", entryName);

				refBuilder->get_widget("btnAddAccept", btnAddPhase);
				refBuilder->get_widget("btnAddCancel", btnCancelPhase);

				refBuilder->get_widget("btnColourSelect", btnColour);

				//set up handlers
				gtkwDialogAddME = this; //pointer to self for use by static handlers
				btnAddPhase->signal_clicked().connect( sigc::ptr_fun(on_btnAccept_clicked) );
				btnCancelPhase->signal_clicked().connect( sigc::ptr_fun(on_btnCancel_clicked) );
	  		}
		}
}

PlotPhaseGtkDialogAddPhasor::~PlotPhaseGtkDialogAddPhasor(){
	delete pDialog;
	delete entryMag;
	delete entryName;
	delete btnAddPhase;
	delete btnCancelPhase;
	delete btnColour;
}

void PlotPhaseGtkDialogAddPhasor::show(){
	pDialog->show();
}


