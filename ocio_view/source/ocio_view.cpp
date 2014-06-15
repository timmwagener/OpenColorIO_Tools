




//Description
//-----------------------------------------------
/*
Implementation of OCIO_view
*/





//Include
//-----------------------------------------------

#include "ocio_view.h"







//Constructor / Destructor
//-----------------------------------------------
//-----------------------------------------------

//Constructor
OCIO_view::OCIO_view(QWidget* parent) :
    QMainWindow(parent)
{
    //set_initial_pointer_addresses
	set_initial_pointer_addresses();

	//set_config_from_env
	set_config_from_env();

	//set_colorspaces
	set_colorspaces();

	//set_display_colorspaces
	set_display_views();
	
	//Setup Ui
	ui->setupUi(this);

	//setup_ui
	setup_ui();

	//connect_ui
	connect_ui();

	
	
}

//Destructor
OCIO_view::~OCIO_view()
{
    delete ui;
}






//Public Methods
//-----------------------------------------------
//-----------------------------------------------


//ui
//-----------------------------------------------

//setup_ui
void OCIO_view::setup_ui()
{
	
	//logging
	if(logging < 1)
		std::cout << "Setup UI" << std::endl;

	//set_lbl_image
	set_lbl_image();
	//recreate_main_menu
	recreate_main_menu();
	//set_theme
	set_theme();
	//set_display_views_ui
	set_display_views_ui();
	//set_display_channels_ui
	set_display_channels_ui();
	

};

//connect_ui
void OCIO_view::connect_ui()
{
	//logging
	if(logging < 1)
		std::cout << "Connect UI" << std::endl;

	//config_changed
	connect(this, &OCIO_view::config_changed, this, &OCIO_view::update_config_changed);
	//image_changed
	connect(this, &OCIO_view::image_changed, this, &OCIO_view::update_image_changed);
	//image_colorspace_changed
	connect(this, &OCIO_view::image_colorspace_changed, this, &OCIO_view::update_display);
	//cmbbx_display_views
	connect(ui->cmbbx_display_views, &QComboBox::currentTextChanged, this, &OCIO_view::update_display);
	//dsb_gain
	connect(ui->dsb_gain, SIGNAL(valueChanged(double)), this, SLOT(update_display()));
	//cmbbx_channels
	connect(ui->cmbbx_channels, &QComboBox::currentTextChanged, this, &OCIO_view::update_display);
	//inspect_begin
	connect(lbl_image, &Image_label::inspect_begin, this, &OCIO_view::inspect_begin);
	//inspect_move
	connect(lbl_image, &Image_label::inspect_move, this, &OCIO_view::inspect_move);
	//inspect_end
	connect(lbl_image, &Image_label::inspect_end, this, &OCIO_view::inspect_end);

};

//set_initial_pointer_addresses
void OCIO_view::set_initial_pointer_addresses()
{
	//ui
	ui = new Ui::OCIO_view;
	inspect_view = 0;
	dev = 1;
	logging = 0;
	action_toggle_shaper_colorspace_active = 0;

	//image
	image = 0;
	image_source = 0;
	image_display = 0;
	lbl_image = 0;
	
	//pixels
	pixels_r = 0;
	pixels_g = 0;
	pixels_b = 0;
	pixels_a = 0;
	pixels_source_r = 0;
	pixels_source_g = 0;
	pixels_source_b = 0;
	pixels_source_a = 0;
	pixels_display_r = 0;
	pixels_display_g = 0;
	pixels_display_b = 0;
	pixels_display_a = 0;
	
	//ocio
	config = 0;
};

//set_lbl_image
void OCIO_view::set_lbl_image()
{
	//logging
	if(logging < 1)
		std::cout << "Set lbl_image" << std::endl;

	//delete temp label
	ui->lbl_image_temp->deleteLater();

	//lbl_image
	lbl_image = new Image_label();

	//add to lyt_image
	ui->lyt_image->addWidget(lbl_image);

	return;
};

//recreate_main_menu
void OCIO_view::recreate_main_menu()
{
	//logging
	if(logging < 1)
		std::cout << "Recreate main menu" << std::endl;

	//clear main menu
	ui->mb_main->clear();
	
	//create_file_menu
	create_file_menu();

	//create_ocio_menu
	create_ocio_menu();

	//create_bake_menu
	create_bake_menu();

	//create_dev_menu
	if(dev)
		create_dev_menu();

};

//create_file_menu
void OCIO_view::create_file_menu()
{
	//menu_file
	//-------------------------------------------------------
	QMenu* mnu_file;
    mnu_file = new QMenu();
	mnu_file->setTitle("File");
	ui->mb_main->addMenu(mnu_file);

	//action_load_image
	QAction* action_load_image = new QAction("Load image", mnu_file);
	mnu_file->addAction(action_load_image);
	connect(action_load_image, SIGNAL(triggered()), this, SLOT(set_image()));

	
	//seperator
	mnu_file->addSeparator();
	
	
	//menu_logging
	//-------------------------------------------------------
	QMenu* menu_logging;
    menu_logging = new QMenu();
	menu_logging->setTitle("Logging");
	mnu_file->addMenu(menu_logging);
	
	
	//signal_mapper
	QSignalMapper* signal_mapper = new QSignalMapper(this);

	//qag_logging
	QActionGroup* qag_logging = new QActionGroup(menu_logging);
	
	
	//action_set_logging_debug
	QAction* action_set_logging_debug = new QAction("Debug", menu_logging);
	menu_logging->addAction(action_set_logging_debug);
	//signal mapping
	signal_mapper->setMapping(action_set_logging_debug, 0);
	connect(action_set_logging_debug, SIGNAL(triggered()),
			signal_mapper, SLOT(map()));
	connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(set_logging(int)));
	//set checkable and checked
	action_set_logging_debug->setCheckable(true);
	action_set_logging_debug->setChecked(true);
	action_set_logging_debug->triggered();
	//add to actiongroup
	action_set_logging_debug->setActionGroup(qag_logging);

	
	//action_set_logging_info
	QAction* action_set_logging_info = new QAction("Info", menu_logging);
	menu_logging->addAction(action_set_logging_info);
	//signal mapping
	signal_mapper->setMapping(action_set_logging_info, 1);
	connect(action_set_logging_info, SIGNAL(triggered()),
			signal_mapper, SLOT(map()));
	connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(set_logging(int)));
	//set checkable and checked
	action_set_logging_info->setCheckable(true);
	//add to actiongroup
	action_set_logging_info->setActionGroup(qag_logging);
	

	
};

//create_ocio_menu
void OCIO_view::create_ocio_menu()
{
	//menu_ocio
	//-------------------------------------------------------
	QMenu* mnu_ocio;
    mnu_ocio = new QMenu();
	mnu_ocio->setTitle("OCIO");
	ui->mb_main->addMenu(mnu_ocio);

	//action_refresh_config
	QAction* action_refresh_config = new QAction("Refresh default config ($OCIO)", mnu_ocio);
	mnu_ocio->addAction(action_refresh_config);
	connect(action_refresh_config, SIGNAL(triggered()), this, SLOT(set_config_from_env()));

	//action_set_config_from_file
	QAction* action_set_config_from_file = new QAction("Set config from file", mnu_ocio);
	mnu_ocio->addAction(action_set_config_from_file);
	connect(action_set_config_from_file, SIGNAL(triggered()), this, SLOT(set_config_from_file()));

	//seperator
	mnu_ocio->addSeparator();
	
	
	//menu_input_colorspace
	//-------------------------------------------------------
	QMenu* menu_input_colorspace;
    menu_input_colorspace = new QMenu();
	menu_input_colorspace->setTitle("Input colorspace");
	mnu_ocio->addMenu(menu_input_colorspace);

	
	//qag_input_colorspace
	QActionGroup* qag_input_colorspace = new QActionGroup(menu_input_colorspace);
	
	
	//iterate vec_colorspace and create action
	
	//action_set_input_colorspace
	QAction* action_set_input_colorspace;

	//signal_mapper
	QSignalMapper* signal_mapper; 
	
	//target_colorspace_name
	std::string target_colorspace_name;
	
	
	for(int index = 0; index < vec_colorspaces.size(); index++)
	{
		//target_colorspace_name
		target_colorspace_name = vec_colorspaces[index];
		
		//action_set_input_colorspace
		action_set_input_colorspace = new QAction(target_colorspace_name.c_str(), menu_input_colorspace);
		menu_input_colorspace->addAction(action_set_input_colorspace);

		//set mapping
		signal_mapper = new QSignalMapper(this);
		signal_mapper->setMapping(action_set_input_colorspace, index);

		//signal -> signal_mapper
		connect(action_set_input_colorspace, SIGNAL(triggered()),
				signal_mapper, SLOT(map()));
		//signal_mapper -> slot
		connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(set_input_colorspace(int)));

		//checkable and checked (if first)
		action_set_input_colorspace->setCheckable(true);
		if(index < 1)
		{
			action_set_input_colorspace->setChecked(true);
			action_set_input_colorspace->triggered();
		};

		//add to actiongroup
		action_set_input_colorspace->setActionGroup(qag_input_colorspace);
	};


	//menu_output_colorspace
	//-------------------------------------------------------
	QMenu* menu_output_colorspace;
    menu_output_colorspace = new QMenu();
	menu_output_colorspace->setTitle("Output colorspace");
	mnu_ocio->addMenu(menu_output_colorspace);

	//qag_output_colorspace
	QActionGroup* qag_output_colorspace = new QActionGroup(menu_output_colorspace);
	
	//iterate vec_colorspace and create action
	
	//action_set_output_colorspace
	QAction* action_set_output_colorspace;
	
	for(int index = 0; index < vec_colorspaces.size(); index++)
	{
		//target_colorspace_name
		target_colorspace_name = vec_colorspaces[index];
		
		//action_set_input_colorspace
		action_set_output_colorspace = new QAction(target_colorspace_name.c_str(), menu_output_colorspace);
		menu_output_colorspace->addAction(action_set_output_colorspace);

		//set mapping
		signal_mapper = new QSignalMapper(this);
		signal_mapper->setMapping(action_set_output_colorspace, index);

		//signal -> signal_mapper
		connect(action_set_output_colorspace, SIGNAL(triggered()),
				signal_mapper, SLOT(map()));
		//signal_mapper -> slot
		connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(set_output_colorspace(int)));

		//checkable and checked (if first)
		action_set_output_colorspace->setCheckable(true);
		if(index < 1)
		{
			action_set_output_colorspace->setChecked(true);
			action_set_output_colorspace->triggered();
		};

		//add to actiongroup
		action_set_output_colorspace->setActionGroup(qag_output_colorspace);
	};

	//action_transform_input_to_output_colorspace
	QAction* action_transform_input_to_output_colorspace = new QAction("Transform colorspace", mnu_ocio);
	mnu_ocio->addAction(action_transform_input_to_output_colorspace);
	connect(action_transform_input_to_output_colorspace, SIGNAL(triggered()), this, SLOT(transform_input_to_output_colorspace()));

	//seperator
	mnu_ocio->addSeparator();

	//action_restore_image
	QAction* action_restore_image = new QAction("Restore image", mnu_ocio);
	mnu_ocio->addAction(action_restore_image);
	connect(action_restore_image, SIGNAL(triggered()), this, SLOT(restore_image()));

	//seperator
	mnu_ocio->addSeparator();
	
	//action_print_config_info
	QAction* action_print_config_info = new QAction("Print config info", mnu_ocio);
	mnu_ocio->addAction(action_print_config_info);
	connect(action_print_config_info, SIGNAL(triggered()), this, SLOT(print_config_info()));
};

//create_bake_menu
void OCIO_view::create_bake_menu()
{
	//menu_bake
	//-------------------------------------------------------
	QMenu* mnu_bake;
    mnu_bake = new QMenu();
	mnu_bake->setTitle("Bake");
	ui->mb_main->addMenu(mnu_bake);
	
	
	//mnu_lut_type
	//-------------------------------------------------------
	QMenu* mnu_lut_type;
    mnu_lut_type = new QMenu();
	mnu_lut_type->setTitle("LUT Type");
	mnu_bake->addMenu(mnu_lut_type);
	
	
	//signal_mapper
	QSignalMapper* signal_mapper = new QSignalMapper(this);

	//qag_lut_type
	QActionGroup* qag_lut_type = new QActionGroup(mnu_lut_type);
	
	
	//action_set_lut_type_1d
	QAction* action_set_lut_type_1d = new QAction("1D", mnu_lut_type);
	mnu_lut_type->addAction(action_set_lut_type_1d);
	//signal mapping
	signal_mapper->setMapping(action_set_lut_type_1d, 0);
	connect(action_set_lut_type_1d, SIGNAL(triggered()), signal_mapper, SLOT(map()));
	connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(set_lut_type(int)));
	//set checkable and checked
	action_set_lut_type_1d->setCheckable(true);
	action_set_lut_type_1d->setChecked(true);
	action_set_lut_type_1d->triggered();
	//add to actiongroup
	action_set_lut_type_1d->setActionGroup(qag_lut_type);

	
	//action_set_lut_type_3d
	QAction* action_set_lut_type_3d = new QAction("3D", mnu_lut_type);
	mnu_lut_type->addAction(action_set_lut_type_3d);
	//signal mapping
	signal_mapper->setMapping(action_set_lut_type_3d, 1);
	connect(action_set_lut_type_3d, SIGNAL(triggered()), signal_mapper, SLOT(map()));
	connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(set_lut_type(int)));
	//set checkable and checked
	action_set_lut_type_3d->setCheckable(true);
	//add to actiongroup
	action_set_lut_type_3d->setActionGroup(qag_lut_type);




	//mnu_lut_format
	//-------------------------------------------------------
	QMenu* mnu_lut_format;
    mnu_lut_format = new QMenu();
	mnu_lut_format->setTitle("LUT Format");
	mnu_bake->addMenu(mnu_lut_format);

	
	//qag_lut_format
	QActionGroup* qag_lut_format = new QActionGroup(mnu_lut_format);
	
	
	//iterate lut_format_count and create action
	
	//action_set_lut_format
	QAction* action_set_lut_format;
	
	//lut_format_count
	int lut_format_count = OCIO::Baker::getNumFormats();

	//format_name
	const char* format_name;
	//format_extension
	const char* format_extension;
	
	
	for(int index = 0; index < lut_format_count; index++)
	{
		//set format_name, format_extension
		format_name = OCIO::Baker::getFormatNameByIndex(index);
		format_extension = OCIO::Baker::getFormatExtensionByIndex(index);

		//format_entry
		boost::format format_entry("%s - (%s)");
		format_entry % format_name % format_extension;
		
		//action_set_lut_format
		action_set_lut_format = new QAction(format_entry.str().c_str(), mnu_lut_format);
		mnu_lut_format->addAction(action_set_lut_format);

		//set mapping
		signal_mapper = new QSignalMapper(this);
		signal_mapper->setMapping(action_set_lut_format, index);

		//signal -> signal_mapper
		connect(action_set_lut_format, SIGNAL(triggered()), signal_mapper, SLOT(map()));
		//signal_mapper -> slot
		connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(set_lut_format(int)));

		//checkable and checked (if first)
		action_set_lut_format->setCheckable(true);
		if(index < 1)
		{
			action_set_lut_format->setChecked(true);
			action_set_lut_format->triggered();
		};

		//add to actiongroup
		action_set_lut_format->setActionGroup(qag_lut_format);
	};




	//mnu_shaper_colorspace
	//-------------------------------------------------------
	QMenu* mnu_shaper_colorspace;
    mnu_shaper_colorspace = new QMenu();
	mnu_shaper_colorspace->setTitle("Shaper colorspace");
	mnu_bake->addMenu(mnu_shaper_colorspace);

	//action_toggle_shaper_colorspace_active
	if(action_toggle_shaper_colorspace_active)
		delete action_toggle_shaper_colorspace_active;
	action_toggle_shaper_colorspace_active = new QAction("Toggle shaper colorspace", mnu_shaper_colorspace);
	mnu_shaper_colorspace->addAction(action_toggle_shaper_colorspace_active);
	connect(action_toggle_shaper_colorspace_active, SIGNAL(changed()), this, SLOT(toggle_shaper_colorspace_active()));
	//set checkable
	action_toggle_shaper_colorspace_active->setCheckable(true);
	action_toggle_shaper_colorspace_active->changed();

	
	//seperator
	mnu_shaper_colorspace->addSeparator();

	
	
	
	//shaper_colorspace
	
	//qag_shaper_colorspace
	QActionGroup* qag_shaper_colorspace = new QActionGroup(mnu_shaper_colorspace);
	
	
	//iterate vec_colorspace and create action
	
	//action_set_shaper_colorspace
	QAction* action_set_shaper_colorspace;
	
	//target_colorspace_name
	std::string target_colorspace_name;
	
	
	for(int index = 0; index < vec_colorspaces.size(); index++)
	{
		//target_colorspace_name
		target_colorspace_name = vec_colorspaces[index];
		
		//action_set_input_colorspace
		action_set_shaper_colorspace = new QAction(target_colorspace_name.c_str(), mnu_shaper_colorspace);
		mnu_shaper_colorspace->addAction(action_set_shaper_colorspace);

		//set mapping
		signal_mapper = new QSignalMapper(this);
		signal_mapper->setMapping(action_set_shaper_colorspace, index);

		//signal -> signal_mapper
		connect(action_set_shaper_colorspace, SIGNAL(triggered()),
				signal_mapper, SLOT(map()));
		//signal_mapper -> slot
		connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(set_shaper_colorspace(int)));

		//checkable and checked (if first)
		action_set_shaper_colorspace->setCheckable(true);
		if(index < 1)
		{
			action_set_shaper_colorspace->setChecked(true);
			action_set_shaper_colorspace->triggered();
		};

		//add to actiongroup
		action_set_shaper_colorspace->setActionGroup(qag_shaper_colorspace);
	};

	//action_bake_lut
	QAction* action_bake_lut = new QAction("Bake LUT", mnu_bake);
	mnu_bake->addAction(action_bake_lut);
	connect(action_bake_lut, SIGNAL(triggered()), this, SLOT(bake_lut()));

	
	
	//seperator
	mnu_bake->addSeparator();

	
	//action_print_bake_setup
	QAction* action_print_bake_setup = new QAction("Print bake setup", mnu_bake);
	mnu_bake->addAction(action_print_bake_setup);
	connect(action_print_bake_setup, SIGNAL(triggered()), this, SLOT(print_bake_setup()));

};

//create_dev_menu
void OCIO_view::create_dev_menu()
{
	//menu_dev
	//-------------------------------------------------------
	
	//menu_dev
	QMenu* mnu_dev;
	mnu_dev = new QMenu();
	mnu_dev->setTitle("Dev");
	ui->mb_main->addMenu(mnu_dev);

	//action_test_method
	QAction* action_test_method = new QAction("Test method", mnu_dev);
	mnu_dev->addAction(action_test_method);
	connect(action_test_method, SIGNAL(triggered()), this, SLOT(test_method()));

	//action_ocio_test_method
	QAction* action_ocio_test_method = new QAction("OCIO test method", mnu_dev);
	mnu_dev->addAction(action_ocio_test_method);
	connect(action_ocio_test_method, SIGNAL(triggered()), this, SLOT(ocio_test_method()));

	//action_print_pixel_values
	QAction* action_print_pixel_values = new QAction("Print pixel values", mnu_dev);
	mnu_dev->addAction(action_print_pixel_values);
	connect(action_print_pixel_values, SIGNAL(triggered()), this, SLOT(print_pixel_values()));

	//action_print_first_pixel_values
	QAction* action_print_first_pixel_values = new QAction("Print first pixel values", mnu_dev);
	mnu_dev->addAction(action_print_first_pixel_values);
	connect(action_print_first_pixel_values, SIGNAL(triggered()), this, SLOT(print_first_pixel_values()));

	//action_print_image_and_label_size
	QAction* action_print_image_and_label_size = new QAction("Print image and label size", mnu_dev);
	mnu_dev->addAction(action_print_image_and_label_size);
	connect(action_print_image_and_label_size, SIGNAL(triggered()), this, SLOT(print_image_and_label_size()));

	//action_print_baker_writers
	QAction* action_print_baker_writers = new QAction("Print baker writers", mnu_dev);
	mnu_dev->addAction(action_print_baker_writers);
	connect(action_print_baker_writers, SIGNAL(triggered()), this, SLOT(print_baker_writers()));
	
};

//set_theme
void OCIO_view::set_theme()
{
	//Dark Fusion
	qApp->setStyle(QStyleFactory::create("Fusion"));
 
	QPalette darkPalette;
	darkPalette.setColor(QPalette::Window, QColor(53,53,53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(25,25,25));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53,53,53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
 
	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);
     
	qApp->setPalette(darkPalette);
 
	qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
};

//set_display_views_ui
void OCIO_view::set_display_views_ui()
{
	//logging
	if(logging < 1)
		std::cout << "Setting display views in UI" << std::endl;
	
	//items_list
	QStringList items_list;

	//iterate and add items
	for(int index = 0; index < vec_display_views.size(); index++)
	{
		items_list << vec_display_views[index].c_str();
	};

	//clear cmbbx
	ui->cmbbx_display_views->clear();

	//set list to cmbbx
	ui->cmbbx_display_views->insertItems(0, items_list);
};

//get_display_view_ui
std::string OCIO_view::get_display_view_ui()
{
	//logging
	if(logging < 1)
		std::cout << "Getting display colorspace from UI" << std::endl;

	return ui->cmbbx_display_views->currentText().toStdString();
};

//set_display_channels_ui
void OCIO_view::set_display_channels_ui()
{
	//logging
	if(logging < 1)
		std::cout << "Setting display channels in UI" << std::endl;
	
	//items_list
	QStringList items_list;

	//add channels
	items_list << "rgb" << "r" << "g" << "b" << "a" << "luma";
	

	//clear cmbbx
	ui->cmbbx_channels->clear();

	//set list to cmbbx
	ui->cmbbx_channels->insertItems(0, items_list);
};

//get_display_channels_ui
void OCIO_view::get_display_channels_ui(int (&channel_hot)[4])
{
	//channel
	std::string channel(ui->cmbbx_channels->currentText().toStdString());

	//tmp
	std::cout << "Channel: " << channel << std::endl;

	

	//temp_channel_hot
	//rgb
	if(channel.compare("rgb") == 0)
	{
		channel_hot[0] = 1;
		channel_hot[1] = 1;
		channel_hot[2] = 1;
		channel_hot[3] = 1;
	}
	//r
	else if(channel.compare("r") == 0)
	{
		channel_hot[0] = 1;
		channel_hot[1] = 0;
		channel_hot[2] = 0;
		channel_hot[3] = 0;
	}
	//g
	else if(channel.compare("g") == 0)
	{
		channel_hot[0] = 0;
		channel_hot[1] = 1;
		channel_hot[2] = 0;
		channel_hot[3] = 0;
	}
	//b
	else if(channel.compare("b") == 0)
	{
		channel_hot[0] = 0;
		channel_hot[1] = 0;
		channel_hot[2] = 1;
		channel_hot[3] = 0;
	}
	//a
	else if(channel.compare("a") == 0)
	{
		channel_hot[0] = 0;
		channel_hot[1] = 0;
		channel_hot[2] = 0;
		channel_hot[3] = 1;
	}
	else
	{
		channel_hot[0] = 1;
		channel_hot[1] = 1;
		channel_hot[2] = 1;
		channel_hot[3] = 0;
	}
};

//get_file_path
std::string OCIO_view::get_file_path()
{
	//file_path
	std::string file_path;

	//getOpenFileName
	file_path = QFileDialog::getOpenFileName().toStdString();

	return file_path;
};

//path_exists
bool OCIO_view::path_exists(std::string& file_path)
{
	//file
	QFile qf_path(QString(file_path.c_str()));

	//file exists
	if(!qf_path.exists())
	{
		std::cout << "File Path: " << file_path << " does not exists. Returning False" << std::endl;
		return false;
	};

	return true;
};

//config_exists
bool OCIO_view::config_exists()
{
	//config exists
	if(!config)
	{
		//logging
		if(logging < 1)
			std::cout << "Config not set" << std::endl;
		return false;
	};

	return true;
};

//get_gain
float OCIO_view::get_gain()
{
	float gain;
	gain = float(ui->dsb_gain->value());
	return gain;
};

//get_lut_type
std::string OCIO_view::get_lut_type()
{
	//lut_type_name
	std::string lut_type_name;

	//assign based on value
	//1D
	if(lut_type == 0)
		lut_type_name = "1D";
	//3D
	else
		lut_type_name = "3D";

	return lut_type_name;
};

//get_lut_format
std::string OCIO_view::get_lut_format()
{
	//lut_format_name
	std::string lut_format_name(OCIO::Baker::getFormatNameByIndex(lut_format));

	return lut_format_name;
};


//image
//-----------------------------------------------

//get_image
QPixmap* OCIO_view::get_image()
{
	return image;
};

//get_image_pixel_count
int OCIO_view::get_image_pixel_count(QPixmap* temp_image)
{
	//null ptr
	if(!temp_image)
	{
		//logging
		if(logging < 1)
			std::cout << "Image does not exist." << std::endl;
		
		return 0;
	};
	
	//pixel_count
	int image_width = temp_image->width();
	int image_height = temp_image->height();
	int pixel_count = image_width * image_height;
	return pixel_count;
};

//image_exists
bool OCIO_view::image_exists()
{
	//image exists
	if(!image)
	{
		//logging
		if(logging < 1)
			std::cout << "Image not set" << std::endl;
		return false;
	};

	return true;
};

//image_display_exists
bool OCIO_view::image_display_exists()
{
	//image_display exists
	if(!image_display)
	{
		//logging
		if(logging < 1)
			std::cout << "image_display not set" << std::endl;
		return false;
	};

	return true;
};

//get_image_scale_factor
void OCIO_view::get_image_scale_factor(float (&scale_factors)[2])
{
	//scale_factor_x
	float scale_factor_x = static_cast<float>(image->width()) / static_cast<float>(lbl_image->width());
	scale_factors[0] = scale_factor_x;
	//scale_factor_y
	float scale_factor_y = static_cast<float>(image->height()) / static_cast<float>(lbl_image->height());
	scale_factors[1] = scale_factor_y;
};

//get_color_at_position
QColor OCIO_view::get_color_at_position(QPixmap*& temp_pixmap, int pos_x, int pos_y)
{
	//q_image
	QImage q_image = temp_pixmap->toImage();
	//qrgb
	QRgb qrgb = q_image.pixel(pos_x, pos_y);
	//color
	QColor color = QColor::fromRgba(qrgb);

	return color;
};


//pixels
//-----------------------------------------------

//set_pixels_from_image
void OCIO_view::set_pixels_from_image(QPixmap* new_image, float*& pixels_temp_r, 
										float*& pixels_temp_g, float*& pixels_temp_b, 
										float*& pixels_temp_a)
{
	//logging
	if(logging < 1)
		std::cout << "set_pixels_from_image" << std::endl;
	
	//image exists
	if(!image_exists())
		return;
	
	//q_image
	QImage q_image = new_image->toImage();
	
	//vec_pixels
	std::vector<QColor> vec_pixels;

	//iterate and fill vector
	int image_width = new_image->width();
	int image_height = new_image->height();
	for(int row = 0; row < image_height; row++){
		for(int column = 0; column < image_width; column++)
		{
			//qrgb
			QRgb qrgb = q_image.pixel(column, row);
			//color
			QColor color = QColor::fromRgba(qrgb);
			//append to vector
			vec_pixels.push_back(color);
		};
	};


	//delete old pixel arrays
	if(pixels_temp_r)
		delete [] pixels_temp_r;
	if(pixels_temp_g)
		delete [] pixels_temp_g;
	if(pixels_temp_b)
		delete [] pixels_temp_b;
	if(pixels_temp_a)
		delete [] pixels_temp_a;
	
	//reallocate pixels
	pixels_temp_r = new float[vec_pixels.size()];
	pixels_temp_g = new float[vec_pixels.size()];
	pixels_temp_b = new float[vec_pixels.size()];
	pixels_temp_a = new float[vec_pixels.size()];
	
	//iterate and fill pixels
	for(int index = 0; index < vec_pixels.size(); index++)
	{
		pixels_temp_r[index] = float(vec_pixels[index].red()) / float(EIGHT_BIT_UNSIGNED_INT);
		pixels_temp_g[index] = float(vec_pixels[index].green()) / float(EIGHT_BIT_UNSIGNED_INT);
		pixels_temp_b[index] = float(vec_pixels[index].blue()) / float(EIGHT_BIT_UNSIGNED_INT);
		pixels_temp_a[index] = float(vec_pixels[index].alpha()) / float(EIGHT_BIT_UNSIGNED_INT);
	};

	
};

//copy_pixels
void OCIO_view::copy_pixels(float*& pixels_destination_r, float*& pixels_destination_g, float*& pixels_destination_b, float*& pixels_destination_a,
					float*& pixels_target_r, float*& pixels_target_g, float*& pixels_target_b, float*& pixels_target_a)
{
	//logging
	if(logging < 1)
		std::cout << "copy_pixels" << std::endl;

	//image exists
	if(!image_exists())
		return;

	//delete old pixel arrays
	if(pixels_destination_r)
		delete [] pixels_destination_r;
	if(pixels_destination_g)
		delete [] pixels_destination_g;
	if(pixels_destination_b)
		delete [] pixels_destination_b;
	if(pixels_destination_a)
		delete [] pixels_destination_a;


	//pixel_count
	int pixel_count = get_image_pixel_count(image);
	//byte_size
	size_t byte_size = pixel_count * sizeof(float);


	//reallocate pixels
	pixels_destination_r = new float[pixel_count];
	pixels_destination_g = new float[pixel_count];
	pixels_destination_b = new float[pixel_count];
	pixels_destination_a = new float[pixel_count];

	

	//memcpy
	memcpy(pixels_destination_r, pixels_target_r, byte_size);
	memcpy(pixels_destination_g, pixels_target_g, byte_size);
	memcpy(pixels_destination_b, pixels_target_b, byte_size);
	memcpy(pixels_destination_a, pixels_target_a, byte_size);
};

//create_pixmap_from_pixels
QPixmap OCIO_view::create_pixmap_from_pixels(int width, int height, 
												float*& pixels_temp_r, float*& pixels_temp_g, 
												float*& pixels_temp_b, float*& pixels_temp_a)
{
	//logging
	if(logging < 1)
		std::cout << "Create pixmap from pixels" << std::endl;
	
	//q_pixmap
	QPixmap* temp_pixmap = new QPixmap(width, height);

	//q_image
	QImage q_image = temp_pixmap->toImage();

	//iterate and set color
	int flat_index;
	for(int row = 0; row < height; row++){
		for(int column = 0; column < width; column++)
		{
			//flat_index
			flat_index = (row*width+column);
			//set color
			q_image.setPixel(column, 
								row, 
								qRgba(int(pixels_temp_r[flat_index]*255), 
									int(pixels_temp_g[flat_index]*255), 
									int(pixels_temp_b[flat_index]*255),
									255)
									);
		};
	};

	//return_pixmap
	QPixmap return_pixmap(QPixmap::fromImage(q_image));

	//delete temp_pixmap
	delete temp_pixmap;

	return return_pixmap;
};






//ocio
//-----------------------------------------------

//set_colorspaces
void OCIO_view::set_colorspaces()
{
	//config exists?
	if(!config_exists())
		return;
	
	//clear vec_colorspaces
	vec_colorspaces.clear();

	//logging
	if(logging < 1)
		std::cout << "\nConfig colorspaces:" << std::endl;

	//set vec_colorspaces
	for(int index=0; index < config->getNumColorSpaces(); index++)
	{
		//append to vector
		std::string colorspace_name(config->getColorSpaceNameByIndex(index));
		vec_colorspaces.push_back(colorspace_name);

		//logging
		if(logging < 1)
			std::cout << colorspace_name << std::endl;
	};


};

//set_display_views
void OCIO_view::set_display_views()
{
	//config exists?
	if(!config_exists())
		return;

	//default_display
	const char* default_display = config->getDefaultDisplay();

	//num_views
	int num_views = config->getNumViews(default_display); //views are names for display colorspaces

	//view
	const char* view;

	//clear vec_display_colorspaces
	vec_display_views.clear();

	//logging
	if(logging < 1)
		std::cout << "\nDisplay views:" << std::endl;

	//iterate and set vec_display_colorspaces
	for(int index = 0; index < num_views; index++)
	{
		//view
		view = config->getView(default_display, index);
		
		//view_std_string
		std::string view_std_string(view);
		
		//append to vector
		vec_display_views.push_back(view_std_string);
		
		//logging
		if(logging < 1)
			std::cout << view_std_string << std::endl;
	};
};

//transform_colorspace
void OCIO_view::transform_colorspace(const char* input_colorspace_name,
										const char* output_colorspace_name,
										float*& pixels_temp_r, float*& pixels_temp_g, 
										float*& pixels_temp_b, float*& pixels_temp_a,
										bool set_image)
{
	//logging
	if(logging < 1)
		std::cout << "transform colorspace" << std::endl;
	
	try
	{
		//planar_image
		OCIO::PlanarImageDesc planar_image(pixels_temp_r, pixels_temp_g, pixels_temp_b, pixels_temp_a, image->width(), image->height());
		
		//processor
		OCIO::ConstProcessorRcPtr processor = config->getProcessor(input_colorspace_name,
																	output_colorspace_name);

		//apply transform
		processor->apply(planar_image);

		//set_image
		if(set_image)
		{
			//q_pixmap
			QPixmap q_pixmap = create_pixmap_from_pixels(image->width(), image->height(), 
															pixels_temp_r, pixels_temp_g, 
															pixels_temp_b, pixels_temp_a);
			//set image
			if(image)
				delete image;
			image = new QPixmap(q_pixmap);

		};

	}
	catch(OCIO::Exception & exception)
	{
		std::cerr << "OpenColorIO Error: " << exception.what() << std::endl;
	}
};

//transform_colorspace
void OCIO_view::transform_colorspace(OCIO::DisplayTransformRcPtr& temp_display_transform,
										float*& pixels_temp_r, float*& pixels_temp_g, 
										float*& pixels_temp_b, float*& pixels_temp_a,
										bool set_image)
{
	//logging
	if(logging < 1)
		std::cout << "transform colorspace" << std::endl;
	
	try
	{
		//planar_image
		OCIO::PlanarImageDesc planar_image(pixels_temp_r, pixels_temp_g, pixels_temp_b, pixels_temp_a, image->width(), image->height());
		
		//processor
		OCIO::ConstProcessorRcPtr processor = config->getProcessor(temp_display_transform);

		//apply transform
		processor->apply(planar_image);

		//set_image
		if(set_image)
		{
			//q_pixmap
			QPixmap q_pixmap = create_pixmap_from_pixels(image->width(), image->height(), 
															pixels_temp_r, pixels_temp_g, 
															pixels_temp_b, pixels_temp_a);
			//set image
			if(image)
				delete image;
			image = new QPixmap(q_pixmap);

		};
	}
	catch(OCIO::Exception & exception)
	{
		std::cerr << "OpenColorIO Error: " << exception.what() << std::endl;
	}
};

//get_colorspace_from_role
OCIO::ConstColorSpaceRcPtr OCIO_view::get_colorspace_from_role(const char* role)
{
	//logging
	if(logging < 1)
		std::cout << "Getting colorspace from role" << std::endl;
	
	//config exists
	if(!config_exists())
		return 0;

	//role of name exists
	if(!config->hasRole(role))
	{
		//logging
		if(logging < 1)
			std::cout << "Config does not have role: " << role << std::endl;
		return 0;
	};

	return config->getColorSpace(role);
		
};








//Signals
//-----------------------------------------------
//-----------------------------------------------





//Slots
//-----------------------------------------------
//-----------------------------------------------



//slots_ui
//-----------------------------------------------

//set_input_colorspace
void OCIO_view::set_input_colorspace(int new_input_colorspace)
{
	input_colorspace = new_input_colorspace;
	//logging
	if(logging < 1)
		std::cout << "Set input colorspace to: " << config->getColorSpaceNameByIndex(input_colorspace) << std::endl;
};

//set_shaper_colorspace
void OCIO_view::set_shaper_colorspace(int new_shaper_colorspace)
{
	shaper_colorspace = new_shaper_colorspace;
	//logging
	if(logging < 1)
		std::cout << "Set shaper colorspace to: " << config->getColorSpaceNameByIndex(shaper_colorspace) << std::endl;
};

//set_output_colorspace
void OCIO_view::set_output_colorspace(int new_output_colorspace)
{
	output_colorspace = new_output_colorspace;
	//logging
	if(logging < 1)
		std::cout << "Set output colorspace to: " << config->getColorSpaceNameByIndex(output_colorspace) << std::endl;
};

//set_logging
void OCIO_view::set_logging(int logging_value)
{
	//set logging
	logging = logging_value;

	//logging
	if(logging < 1)
		std::cout << "Set logging to:" << logging << std::endl;

	return;
};

//set_lut_type
void OCIO_view::set_lut_type(int lut_type_value)
{
	
	//logging
	if(logging < 1)
	{
		if(lut_type_value == 0)
		{
			std::cout << "Set LUT type to: 1D"  << std::endl;
		}
		else
		{
			std::cout << "Set LUT type to: 3D"  << std::endl;
		};
	};

	//set lut_type
	lut_type = lut_type_value;

	return;
};

//set_lut_format
void OCIO_view::set_lut_format(int lut_format_value)
{
	//logging
	if(logging < 1)
		std::cout << "Set LUT format to: " << OCIO::Baker::getFormatNameByIndex(lut_format_value) << std::endl;

	//set lut_format
	lut_format = lut_format_value;

	return;
};

//toggle_shaper_colorspace_active
void OCIO_view::toggle_shaper_colorspace_active()
{
	//toggle shaper_colorspace_active
	if(action_toggle_shaper_colorspace_active->isChecked())
		shaper_colorspace_active = true;
	else
		shaper_colorspace_active = false;

	//logging
	if(logging < 1)
		std::cout << "Set shaper colorspace active to: " << shaper_colorspace_active << std::endl;
};




//slots_image
//-----------------------------------------------

//set_image
void OCIO_view::set_image()
{
	//image_path
	std::string image_path = get_file_path();

	//path exists
	if(!path_exists(image_path))
		return;

	//config exists
	if(!config_exists())
		return;
	
	//deallocate old image if not null ptr
	if(image)
		delete image;

	//image
	image = new QPixmap(QString(image_path.c_str()));

	//emit image_changed
	if(image_exists())
		emit image_changed();
};

//restore_image
void OCIO_view::restore_image()
{
	//logging
	if(logging < 1)
		std::cout << "Displaying image" << std::endl;

	//image exists
	if(!image_exists())
		return;

	//copy pixels_source to pixels
	copy_pixels(pixels_r, pixels_g, pixels_b, pixels_a,
				pixels_source_r, pixels_source_g, pixels_source_b, pixels_source_a);

	//set image
	if(image)
		delete image;
	image = new QPixmap(*image_source);

	//emit image_colorspace_changed()
	emit image_colorspace_changed();

	return;
};

//display_image
void OCIO_view::display_image()
{
	//logging
	if(logging < 1)
		std::cout << "Displaying image" << std::endl;

	//image exists
	if(!image_exists())
		return;

	//copy pixels to pixels_display
	copy_pixels(pixels_display_r, pixels_display_g, pixels_display_b, pixels_display_a,
				pixels_r, pixels_g, pixels_b, pixels_a);
	
	//default_display
	const char* default_display = config->getDefaultDisplay();
	
	//display_view_name
	std::string display_view_name(get_display_view_ui());
	std::cout << "Display View: " << display_view_name.c_str() << std::endl;

	//display_colorspace_name
	const char* display_colorspace_name = config->getDisplayColorSpaceName(default_display, display_view_name.c_str());
	std::cout << "Display Colorspace: " << display_colorspace_name << std::endl;

	//display_colorspace_name empty
	if(std::string(display_colorspace_name).compare("") == 0)
	{
		std::cout << "Display Colorspace empty. Not applying display color transform." << std::endl;
		return;
	};

	
	//display_transform
	OCIO::DisplayTransformRcPtr display_transform = OCIO::DisplayTransform::Create();
	display_transform->setInputColorSpaceName(OCIO::ROLE_SCENE_LINEAR);
	display_transform->setDisplay(default_display);
	display_transform->setView(display_view_name.c_str());


	//gain (applied in linear space)
	float gain = get_gain();
	//logging
	if(logging < 1)
		std::cout << "Gain: " << gain << std::endl;
	
	//slope
	const float slope3f[] = { gain, gain, gain };
	OCIO::CDLTransformRcPtr cdl_transform =  OCIO::CDLTransform::Create();
	cdl_transform->setSlope(slope3f);
	display_transform->setLinearCC(cdl_transform);

	
	//channel_hot
	int channel_hot[4];
	get_display_channels_ui(channel_hot);


	//luma_coefs
	float luma_coefs[3];
	config->getDefaultLumaCoefs(luma_coefs);

	//channel & luma matrix transform
	float m44[16];
	float offset[4];
	OCIO::MatrixTransform::View(m44, offset, channel_hot, luma_coefs);

	
	
	//channel_and_luma_transform
	OCIO::MatrixTransformRcPtr channel_and_luma_transform = OCIO::MatrixTransform::Create();
	channel_and_luma_transform->setValue(m44, offset);
	display_transform->setChannelView(channel_and_luma_transform);

	//apply display colorspace
	transform_colorspace(display_transform, 
							pixels_display_r,pixels_display_g, 
							pixels_display_b,pixels_display_a);

	
	//temp_image_display
	QPixmap temp_image_display = create_pixmap_from_pixels(image->width(), image->height(), 
												pixels_display_r, pixels_display_g, 
												pixels_display_b, pixels_display_a);

	//set image_display
	if(image_display)
		delete image_display;
	image_display = new QPixmap(temp_image_display);
	
	//display
	lbl_image->setPixmap(*image_display);
};

//inspect_begin
void OCIO_view::inspect_begin(int pos_x, int pos_y)
{
	//inspect not checked
	if(!ui->chkbx_inspect->isChecked())
	{
		//logging
		if(logging < 1)
			//std::cout << "Inspect checked off" << std::endl;
		return;
	};

	//image exists
	if(!image_exists())
		return;

	//image_scale_factor
	float image_scale_factor[2];
	get_image_scale_factor(image_scale_factor);

	//image_pos_x
	int image_pos_x = static_cast<int>(image_scale_factor[0] * static_cast<float>(pos_x));
	//check extremes
	if(image_pos_x > image->width())
		image_pos_x = image->width();
	if(image_pos_x < 0)
		image_pos_x = 0;
	
	//image_pos_y
	int image_pos_y = static_cast<int>(image_scale_factor[1] * static_cast<float>(pos_y));
	//check extremes
	if(image_pos_y > image->height())
		image_pos_y = image->height();
	if(image_pos_y < 0)
		image_pos_y = 0;

	//color
	QColor color_graded = get_color_at_position(image, image_pos_x, image_pos_y);
	//color_source
	QColor color_source = get_color_at_position(image_source, image_pos_x, image_pos_y);
	//color_display
	QColor color_display = get_color_at_position(image_display, image_pos_x, image_pos_y);
	

	//color
	std::cout << "Color: R: " << color_graded.red()
		<< " G: " << color_graded.green()
		<< " B: " << color_graded.blue()
		<< " A: " << color_graded.alpha()
		<< std::endl;

	//color_source
	std::cout << "Color Source: R: " << color_source.red()
		<< " G: " << color_source.green()
		<< " B: " << color_source.blue()
		<< " A: " << color_source.alpha()
		<< std::endl;

	//color_display
	std::cout << "Color Display: R: " << color_display.red()
		<< " G: " << color_display.green()
		<< " B: " << color_display.blue()
		<< " A: " << color_display.alpha()
		<< std::endl;

	
	
	
	//inspect_view
	inspect_view = new Inspect_view();
	inspect_view->move(this->mapToGlobal(QPoint(pos_x - INSPECT_VIEW_OFFSET_X, pos_y - INSPECT_VIEW_OFFSET_Y)).x(), 
						this->mapToGlobal(QPoint(pos_x - INSPECT_VIEW_OFFSET_X, pos_y - INSPECT_VIEW_OFFSET_Y)).y()
						);

	//color_types
	std::string graded("graded");
	std::string source("source");
	std::string display("display");
	
	//set_color
	inspect_view->set_color(color_graded, std::string(graded));
	inspect_view->set_color(color_source, std::string(source));
	inspect_view->set_color(color_display, std::string(display));


	//show
	inspect_view->show();


	return;
};

//inspect_move
void OCIO_view::inspect_move(int pos_x, int pos_y)
{
	//inspect not checked
	if(!ui->chkbx_inspect->isChecked())
	{
		//logging
		if(logging < 1)
			//std::cout << "Inspect checked off" << std::endl;
		return;
	};

	//image exists
	if(!image_exists())
		return;

	//image_scale_factor
	float image_scale_factor[2];
	get_image_scale_factor(image_scale_factor);

	//image_pos_x
	int image_pos_x = static_cast<int>(image_scale_factor[0] * static_cast<float>(pos_x));
	//check extremes
	if(image_pos_x > image->width())
		image_pos_x = image->width();
	if(image_pos_x < 0)
		image_pos_x = 0;
	
	//image_pos_y
	int image_pos_y = static_cast<int>(image_scale_factor[1] * static_cast<float>(pos_y));
	//check extremes
	if(image_pos_y > image->height())
		image_pos_y = image->height();
	if(image_pos_y < 0)
		image_pos_y = 0;

	//color_graded
	QColor color_graded = get_color_at_position(image, image_pos_x, image_pos_y);
	//color_source
	QColor color_source = get_color_at_position(image_source, image_pos_x, image_pos_y);
	//color_display
	QColor color_display = get_color_at_position(image_display, image_pos_x, image_pos_y);
	

	//color
	std::cout << "Color: R: " << color_graded.red()
		<< " G: " << color_graded.green()
		<< " B: " << color_graded.blue()
		<< " A: " << color_graded.alpha()
		<< std::endl;

	//color_source
	std::cout << "Color Source: R: " << color_source.red()
		<< " G: " << color_source.green()
		<< " B: " << color_source.blue()
		<< " A: " << color_source.alpha()
		<< std::endl;

	//color_display
	std::cout << "Color Display: R: " << color_display.red()
		<< " G: " << color_display.green()
		<< " B: " << color_display.blue()
		<< " A: " << color_display.alpha()
		<< std::endl;

	
	
	
	
	//inspect_view
	inspect_view->move(this->mapToGlobal(QPoint(pos_x - INSPECT_VIEW_OFFSET_X, pos_y - INSPECT_VIEW_OFFSET_Y)).x(), 
						this->mapToGlobal(QPoint(pos_x - INSPECT_VIEW_OFFSET_X, pos_y - INSPECT_VIEW_OFFSET_Y)).y()
						);
	
	//color_types
	std::string graded("graded");
	std::string source("source");
	std::string display("display");
	
	//set_color
	inspect_view->set_color(color_graded, std::string(graded));
	inspect_view->set_color(color_source, std::string(source));
	inspect_view->set_color(color_display, std::string(display));

	return;
};

//inspect_end
void OCIO_view::inspect_end()
{
	//inspect_view
	if(inspect_view)
	{
		delete inspect_view;
		inspect_view = 0;
	};

	return;
};



//slots_pixels
//-----------------------------------------------

//print_pixel_values
void OCIO_view::print_pixel_values()
{
	//q_timer
	QElapsedTimer q_timer;
    q_timer.start();

	//image exists
	if(!image)
	{
		//Log
		std::cout << "No image set. Not displaying pixel values." << std::endl;
		return;
	};

	

	//display pixels
	std::cout << "All pixel values:" << std::endl;
	for(int index = 0; index < get_image_pixel_count(image); index++)
	{
		std::cout << "Red: " << pixels_r[index] 
		<< " Green: " << pixels_g[index]
		<< " Blue: " << pixels_b[index]
		<< " Alpha: " << pixels_a[index]
		<< std::endl;
	};
	
	
	
	//Display elapsed time
	std::cout << "Displaying the color array took: " << q_timer.elapsed()/1000.0 << " sec." << std::endl;

	return;
};

//print_first_pixel_values
void OCIO_view::print_first_pixel_values()
{
	//q_timer
	QElapsedTimer q_timer;
    q_timer.start();

	//image exists
	if(!image)
	{
		//Log
		std::cout << "No image set. Not displaying pixel values." << std::endl;
		return;
	};

	

	//display pixels
	std::cout << "First 10 pixel values:" << std::endl;
	for(int index = 0; index < 10; index++)
	{
		std::cout << "Red: " << pixels_r[index] 
		<< " Green: " << pixels_g[index]
		<< " Blue: " << pixels_b[index]
		<< " Alpha: " << pixels_a[index]
		<< std::endl;
	};
	
	
	
	//Display elapsed time
	std::cout << "Displaying the color array took: " << q_timer.elapsed()/1000.0 << " sec." << std::endl;

	return;
};





//slots_ocio
//-----------------------------------------------

//set_config_from_env
void OCIO_view::set_config_from_env()
{
	try
	{
		//logging
		if(logging < 1)
			std::cout << "Aquiring config from $OCIO" << std::endl;
		//temp_config
		OCIO::ConstConfigRcPtr temp_config;
		temp_config = OCIO::Config::CreateFromEnv();
		
		//logging
		if(logging < 1)
			std::cout << "Sanity check" << std::endl;
		//sanity check
		temp_config->sanityCheck();
		
		//logging
		if(logging < 1)
			std::cout << "Setting config" << std::endl;
		//set config
		config = temp_config;

		//emit signal config_changed
		emit config_changed();

	}
	catch(OCIO::Exception& exception)
	{
		std::cerr << "OpenColorIO Error: " << exception.what() << std::endl;
	}
};

//set_config_from_file
void OCIO_view::set_config_from_file()
{
	try
	{
		//file_path
		std::string file_path = get_file_path();

		//file_path exists
		if(!path_exists(file_path))
			return;
		
		//logging
		if(logging < 1)
			std::cout << "Aquiring config from " << file_path << std::endl;
		//temp_config
		OCIO::ConstConfigRcPtr temp_config;
		temp_config = OCIO::Config::CreateFromFile(file_path.c_str());
		
		//logging
		if(logging < 1)
			std::cout << "Sanity check" << std::endl;
		//sanity check
		temp_config->sanityCheck();
		
		//logging
		if(logging < 1)
			std::cout << "Setting config" << std::endl;
		//set config
		config = temp_config;

		//emit signal config_changed
		emit config_changed();

	}
	catch(OCIO::Exception& exception)
	{
		std::cerr << "OpenColorIO Error: " << exception.what() << std::endl;
	}
};

//print_config_info
void OCIO_view::print_config_info()
{
	//config exists
	if(!config)
	{
		//log
		std::cout << "Config does not exists. Returning" << std::endl;
		return;
	};
	//break
	std::cout << "\n-----------------------------------------------" << std::endl;
	
	//Number color profiles
	std::cout << "Number of color profiles: " << config->getNumColorSpaces() << std::endl;

	//ColorSpace Names
	std::vector<std::string> colorspace_names;
	for(int index=0; index<config->getNumColorSpaces(); index++)
	{
		//append to vector
		std::string colorspace_name(config->getColorSpaceNameByIndex(index));
		colorspace_names.push_back(colorspace_name);
		//display
		std::cout << colorspace_name << std::endl;
	};

	
	//break
	std::cout << "-----------------------------------------------" << std::endl;

	//Number Roles
	std::cout << "Number of roles: " << config->getNumRoles() << std::endl;

	//Role Names
	std::vector<std::string> role_names;
	for(int index=0; index < config->getNumRoles(); index++)
	{
		//append to vector
		std::string role_name(config->getRoleName(index));
		role_names.push_back(role_name);
		//display
		std::cout << role_name << std::endl;
	};


	//break
	std::cout << "-----------------------------------------------" << std::endl;

	//Search Path
	std::cout << "Search Path: " << config->getSearchPath() << std::endl;
	//Working Dir
	std::cout << "Working Dir.: " << config->getWorkingDir() << std::endl;
	//Strict Parsing enabled
	std::cout << "Strict parsing enabled: " << config->isStrictParsingEnabled() << std::endl;

	return;
};

//transform_input_to_output_colorspace
void OCIO_view::transform_input_to_output_colorspace()
{
	//image exists
	if(!image_exists())
		return;

	//input_colorspace_name
	const char* input_colorspace_name = config->getColorSpaceNameByIndex(input_colorspace);

	//output_colorspace_name
	const char* output_colorspace_name = config->getColorSpaceNameByIndex(output_colorspace);
	
	//transform colorspace
	transform_colorspace(input_colorspace_name, output_colorspace_name, 
							pixels_r, pixels_g, 
							pixels_b, pixels_a,
							true);

	//emit image_colorspace_changed()
	emit image_colorspace_changed();
	
	return;
};

//bake_lut
void OCIO_view::bake_lut()
{
	//logging
	if(logging < 1)
		std::cout << "Bake LUT" << std::endl;

	//config exists
	if(!config_exists())
		return;

	//file_name
	QString file_name = QFileDialog::getSaveFileName(this, tr("Save LUT"),
                           "",
                           "");
	
	//file_name empty
	if(file_name.isEmpty())
	{
		std::cout << "File name empty. Not baking LUT" << std::endl;
		return;
	};

	//file_name_formatted
	boost::format file_name_formatted("%s.%s");
	file_name_formatted % file_name.toStdString().c_str() % OCIO::Baker::getFormatExtensionByIndex(lut_format);;

	//print file path
	std::cout << "File name: " << file_name_formatted.str() << std::endl;

	//file
	QFile file(file_name_formatted.str().c_str());

	//file open failed
	if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		std::cout << "File open failed" << std::endl;
		return;
	}



	//bake
	try
	{
		//input_colorspace_name
		const char* input_colorspace_name = config->getColorSpaceNameByIndex(input_colorspace);
		//output_colorspace_name
		const char* output_colorspace_name = config->getColorSpaceNameByIndex(output_colorspace);
		//shaper_colorspace_name
		const char* shaper_colorspace_name = config->getColorSpaceNameByIndex(shaper_colorspace);

		//lut_type_name
		std::string lut_type_name = get_lut_type();
		//lut_format_name
		std::string lut_format_name = get_lut_format();
		
		
		//bake
		OCIO::BakerRcPtr baker = OCIO::Baker::Create();
		baker->setConfig(config);
		
		//Set format and type
		baker->setFormat(lut_format_name.c_str()); // set the houdini type
		baker->setType(lut_type_name.c_str()); // we want a 3D lut
		
		//Set colorspaces
		baker->setInputSpace(input_colorspace_name);
		if(shaper_colorspace_active)
			baker->setShaperSpace(shaper_colorspace_name);
		baker->setTargetSpace(output_colorspace_name);
		
		//out
		std::ostringstream out;
		baker->bake(out);

		//out_string
		std::string out_string(out.str());

		//q_out_string
		QString q_out_string(out_string.c_str());

		//q_out_stream
		QTextStream q_out_stream(&file);
		q_out_stream << q_out_string;

		//close file
		file.close();
		
	}
	catch(OCIO::Exception & exception)
	{
		std::cerr << "OpenColorIO Error: " << exception.what() << std::endl;
		std::cout << "\n" << std::endl;
	}
};

//print_bake_setup
void OCIO_view::print_bake_setup()
{
	//logging
	if(logging < 1)
		std::cout << "Print bake setup" << std::endl;

	//config exists
	if(!config_exists())
		return;

	//input_colorspace_name
	const char* input_colorspace_name = config->getColorSpaceNameByIndex(input_colorspace);
	//output_colorspace_name
	const char* output_colorspace_name = config->getColorSpaceNameByIndex(output_colorspace);
	//shaper_colorspace_name
	const char* shaper_colorspace_name = config->getColorSpaceNameByIndex(shaper_colorspace);

	//lut_type_name
	std::string lut_type_name = get_lut_type();
	//lut_format_name
	std::string lut_format_name = get_lut_format();
		
		
	//print setup
	std::cout << "Bake setup:\n----------------------------" << std::endl;
	std::cout << "Input colorspace: " << input_colorspace_name << std::endl;
	std::cout << "Output colorspace: " << output_colorspace_name << std::endl;
	std::cout << "Shaper colorspace: " << shaper_colorspace_name << std::endl;
	std::cout << "Shaper colorspace enabled: " << shaper_colorspace_active << std::endl;

	std::cout << "LUT type: " << lut_type_name << std::endl;
	std::cout << "LUT format: " << lut_format_name << std::endl;
	std::cout << "----------------------------" << std::endl;
	
	return;
};





//slots_update
//-----------------------------------------------

//update_config_changed
void OCIO_view::update_config_changed()
{
	//set_colorspaces
	set_colorspaces();

	//set_display_views
	set_display_views();

	//recreate_main_menu
	recreate_main_menu();

	//set_display_views_ui
	set_display_views_ui();
};

//update_image_changed
void OCIO_view::update_image_changed()
{
	//set pixels_source
	set_pixels_from_image(image, pixels_source_r, pixels_source_g, pixels_source_b, pixels_source_a);

	//copy source to pixels
	copy_pixels(pixels_r, pixels_g, pixels_b, pixels_a,
				pixels_source_r, pixels_source_g, pixels_source_b, pixels_source_a);

	//temp_image_source
	QPixmap temp_image_source = create_pixmap_from_pixels(image->width(), image->height(), 
															pixels_source_r, pixels_source_g, 
															pixels_source_b, pixels_source_a);
	//set image_source
	if(image_source)
		delete image_source;
	image_source = new QPixmap(temp_image_source);

	//display image
	display_image();

};

//update_display
void OCIO_view::update_display()
{
	//display image
	display_image();
};








//slots_dev
//-----------------------------------------------

//test_method
void OCIO_view::test_method()
{
	std::cout << "test_method" << std::endl;
};

//ocio_test_method
void OCIO_view::ocio_test_method()
{
	//logging
	if(logging < 1)
		std::cout << "OCIO test method" << std::endl;
};

//print_image_and_label_size
void OCIO_view::print_image_and_label_size()
{
	//logging
	if(logging < 1)
		std::cout << "Print image and label size" << std::endl;

	//label_size
	int label_width = lbl_image->width();
	int label_height = lbl_image->height();
	std::cout << "Label width: " << label_width << std::endl;
	std::cout << "Label height: " << label_height << std::endl;

	//tmp
	std::cout << "Label class type: " << lbl_image->metaObject()->className() << std::endl;
	
	//image exists
	if(!image_exists())
		return;

	//image_size
	int image_width = image->width();
	int image_height = image->height();
	std::cout << "Image width: " << image_width << std::endl;
	std::cout << "Image height: " << image_height << std::endl;

	return;
};

//print_baker_writers
void OCIO_view::print_baker_writers()
{
	
	//No config check
	if (!config_exists())
	{
		//logging
		if (logging < 1)
			std::cout << "No config exists. Returning..." << std::endl;

		return;
	}

	//baker
	OCIO::BakerRcPtr baker = OCIO::Baker::Create();
	
	//set config
	baker->setConfig(config);

	//temp
	std::cout << "Number of baker writers: " << baker->getNumFormats() << std::endl;

	//number_formats
	int number_formats = baker->getNumFormats();

	//iterate and print names
	for (int index = 0; index < number_formats; index++)
	{
		std::cout << baker->getFormatNameByIndex(index) << std::endl;
	};

};









//Event Handler
//-----------------------------------------------
//-----------------------------------------------

