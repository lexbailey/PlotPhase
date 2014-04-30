
#ifndef GTKWINDOWS_INCLUDE
#define GTKWINDOWS_INCLUDE
#include <gtkmm.h>
#include <vector>

using namespace std;

//Tree model columns:
class PhasorModelColumns : public Gtk::TreeModel::ColumnRecord
{
  public:

    PhasorModelColumns()
    { add(m_col_name); add(m_col_magnitude); add(m_col_angle);}

    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<double> m_col_magnitude;
    Gtk::TreeModelColumn<double> m_col_angle;
};

typedef struct{
	double magnitude, phase;
	char name[255];
	char colR, colG, colB;
} phaseData;


class PlotPhaseGtkDialogAddPhasor{
	public:

		Gtk::Dialog *pDialog;

		Gtk::Button *btnAddPhase;
		Gtk::Button *btnCancelPhase;

		Gtk::ColorButton *btnColour;

		Gtk::Entry *entryMag;
		Gtk::Entry *entryAng;
		Gtk::Entry *entryName;

		void show();

		PlotPhaseGtkDialogAddPhasor(Glib::RefPtr<Gtk::Builder> refBuilder);
		~PlotPhaseGtkDialogAddPhasor();
};

class PlotPhaseGtkWindows{
	public:

		vector<phaseData> phases;

		PlotPhaseGtkDialogAddPhasor *addDialog;

		//MaterialModelColumns m_Material_Columns;
		Glib::RefPtr<Gtk::Application> app;
		Gtk::Window *pWindow;

		Gtk::Button *btnAddPhase;
		Gtk::Button *btnDelPhase;

		Gtk::Button *btnOpen;
		Gtk::Button *btnSave;

		Gtk::ColorButton *btnColour;

		//Gtk::FileChooserDialog* openDialog;
		Gtk::Entry *entryMag;
		Gtk::Entry *entryAng;
		Gtk::Entry *entryName;

		Gtk::CheckButton *cbArrow;
		Gtk::CheckButton *cbScope;
		Gtk::CheckButton *cbTotal;
		Gtk::CheckButton *cbNames;

		Gtk::TreeView *tvPhasors;

		Gtk::Scale *scaleLine;

		PhasorModelColumns m_Cols;
		Glib::RefPtr<Gtk::ListStore> ListModel;

		void addPhasor(double magnitude, double angle, char name[255], short r, short g, short b);

		int RunMainWindow();

		PlotPhaseGtkWindows(int argc, char **argv);
		~PlotPhaseGtkWindows();
};



#endif
