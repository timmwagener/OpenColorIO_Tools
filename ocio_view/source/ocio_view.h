



#ifndef OCIO_VIEW_H
#define OCIO_VIEW_H

//Description
//-----------------------------------------------
/*
Declaration of OCIO_view
*/



//Constants
//-----------------------------------------------
#define EIGHT_BIT_UNSIGNED_INT 255
#define INSPECT_VIEW_OFFSET_X -40
#define INSPECT_VIEW_OFFSET_Y 0




//Include
//-----------------------------------------------

//STL
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
//Qt
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtGui/QColor>
#include <QtCore/QElapsedTimer>
#include <QtWidgets/QStyleFactory>
#include <QtCore/QSignalMapper>
#include <QtCore/QStringList>
#include <QtWidgets/QActionGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QTextStream>
//Boost
#include "boost/shared_ptr.hpp"
#include <boost/format.hpp>
//Own
#include "image_label.h"
#include "inspect_view.h"
//OCIO
#include <OpenColorIO/OpenColorIO.h>
//From .ui
#include "ui_ocio_view.h"

//OCIO Namespace
namespace OCIO = OCIO_NAMESPACE;


//OCIO_view
//-----------------------------------------------

class OCIO_view : public QMainWindow
{
	Q_OBJECT


public: //Constructor / Destructor
	explicit OCIO_view(QWidget* parent = 0);
    ~OCIO_view();

	
public: //Methods
	
	//ui
	void setup_ui();
	void connect_ui();
	void set_initial_pointer_addresses();
	void set_lbl_image();
	void recreate_main_menu();
	void create_file_menu();
	void create_ocio_menu();
	void create_bake_menu();
	void create_dev_menu();
	void set_theme();
	void set_display_views_ui();
	std::string get_display_view_ui();
	void set_display_channels_ui();
	void get_display_channels_ui(int (&channel_hot)[4]);
	std::string get_file_path();
	bool path_exists(std::string&);
	bool config_exists();
	float get_gain();
	std::string get_lut_type();
	std::string get_lut_format();
	

	//image
	QPixmap* get_image();
	int get_image_pixel_count(QPixmap*);
	bool image_exists();
	bool image_display_exists();
	void get_image_scale_factor(float (&scale_factors)[2]);
	QColor get_color_at_position(QPixmap*&, int, int);
	
	
	//pixels
	void set_pixels_from_image(QPixmap*, float*&, float*&, float*&, float*&);
	void copy_pixels(float*&, float*&, float*&, float*&,
						float*&, float*&, float*&, float*&);
	QPixmap create_pixmap_from_pixels(int, int, float*&, float*&, float*&, float*&);
	
	//ocio
	void set_colorspaces();
	void set_display_views();
	void transform_colorspace(const char*, const char*, 
								float*&, float*&, float*&, float*&,
								bool set_image = false);
	void transform_colorspace(OCIO::DisplayTransformRcPtr&, 
								float*&, float*&, float*&, float*&,
								bool set_image = false);
	OCIO::ConstColorSpaceRcPtr get_colorspace_from_role(const char*);
	
	

signals: //Signals
	void config_changed();
	void image_changed();
	void image_colorspace_changed();


private slots: //Slots
	
	//slots_ui
	void set_input_colorspace(int);
	void set_shaper_colorspace(int);
	void set_output_colorspace(int);
	void set_logging(int);
	void set_lut_type(int);
	void set_lut_format(int);
	void toggle_shaper_colorspace_active();

	//slots_image
	void set_image();
	void restore_image();
	void display_image();
	void inspect_begin(int, int);
	void inspect_move(int, int);
	void inspect_end();
	
	//slots_pixels
	void print_pixel_values();
	void print_first_pixel_values();
	
	//slots_ocio
	void set_config_from_env();
	void set_config_from_file();
	void print_config_info();
	void transform_input_to_output_colorspace();
	void bake_lut();
	void print_bake_setup();

	//slots_update
	void update_config_changed();
	void update_image_changed();
	void update_display();
	

	//slots_dev
	void test_method();
	void ocio_test_method();
	void print_image_and_label_size();
	void print_baker_writers();


public: //Event Handler


protected: //Variables


private: //Variables
	Ui::OCIO_view* ui;
	bool dev;
	int logging;

	Image_label* lbl_image;
	QPixmap* image;
	QPixmap* image_source;
	QPixmap* image_display;
	
	float* pixels_r, *pixels_g, *pixels_b, *pixels_a;
	float* pixels_source_r, *pixels_source_g, *pixels_source_b, *pixels_source_a;
	float* pixels_display_r, *pixels_display_g, *pixels_display_b, *pixels_display_a;
	
	OCIO::ConstConfigRcPtr config;
	std::vector<std::string> vec_colorspaces;
	std::vector<std::string> vec_display_views;
	
	int input_colorspace;
	int shaper_colorspace;
	int output_colorspace;
	int lut_type;
	int lut_format;
	bool shaper_colorspace_active;
	QAction* action_toggle_shaper_colorspace_active;

	Inspect_view* inspect_view;
};

#endif