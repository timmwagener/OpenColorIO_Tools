




//Description
//-----------------------------------------------
/*
Implementation of Inspect_view
*/





//Include
//-----------------------------------------------

#include "inspect_view.h"







//Constructor / Destructor
//-----------------------------------------------
//-----------------------------------------------

//Constructor
Inspect_view::Inspect_view(QWidget* parent) :
    QWidget(parent)
{
    //set_initial_pointer_addresses
	set_initial_pointer_addresses();

	//Setup Ui
	ui->setupUi(this);

	//setup_ui
	setup_ui();

	//connect_ui
	connect_ui();
}

//Destructor
Inspect_view::~Inspect_view()
{
    delete ui;
}






//Public Methods
//-----------------------------------------------
//-----------------------------------------------

//set_initial_pointer_addresses
void Inspect_view::set_initial_pointer_addresses()
{
	//ui
	ui = new Ui::Inspect_view;
	dev = true;
	logging = 0;
};

//setup_ui
void Inspect_view::setup_ui()
{
	//set attrs
	setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_NoSystemBackground, true);
	setAttribute(Qt::WA_TranslucentBackground, true);
	setAttribute(Qt::WA_PaintOnScreen);
};

//connect_ui
void Inspect_view::connect_ui()
{

};

//set_color
void Inspect_view::set_color(QColor& color, std::string color_type)
{
	//set color bars and labels based on color_type
	//graded
	if(color_type.compare("graded") == 0)
	{
		//toplevel bars
		set_color_label_top(color, ui->lbl_color_graded);
		
		//set color_bars

		//rrrgggbbb
		set_color_bar(color, ui->wdgt_rrrgggbbb_graded_r, "red");
		set_color_bar(color, ui->wdgt_rrrgggbbb_graded_g, "green");
		set_color_bar(color, ui->wdgt_rrrgggbbb_graded_b, "blue");
		//rgbrgbrgb
		set_color_bar(color, ui->wdgt_rgbrgbrgb_graded_r, "red");
		set_color_bar(color, ui->wdgt_rgbrgbrgb_graded_g, "green");
		set_color_bar(color, ui->wdgt_rgbrgbrgb_graded_b, "blue");

		//lbl colors
		set_color_label(color, ui->lbl_graded_r, "red");
		set_color_label(color, ui->lbl_graded_g, "green");
		set_color_label(color, ui->lbl_graded_b, "blue");
	}
	//source
	else if(color_type.compare("source") == 0)
	{
		//toplevel bars
		set_color_label_top(color, ui->lbl_color_source);
		
		//set color_bars

		//rrrgggbbb
		set_color_bar(color, ui->wdgt_rrrgggbbb_source_r, "red");
		set_color_bar(color, ui->wdgt_rrrgggbbb_source_g, "green");
		set_color_bar(color, ui->wdgt_rrrgggbbb_source_b, "blue");
		//rgbrgbrgb
		set_color_bar(color, ui->wdgt_rgbrgbrgb_source_r, "red");
		set_color_bar(color, ui->wdgt_rgbrgbrgb_source_g, "green");
		set_color_bar(color, ui->wdgt_rgbrgbrgb_source_b, "blue");

		//lbl colors
		set_color_label(color, ui->lbl_source_r, "red");
		set_color_label(color, ui->lbl_source_g, "green");
		set_color_label(color, ui->lbl_source_b, "blue");
	}
	//display
	else if(color_type.compare("display") == 0)
	{
		//toplevel bars
		set_color_label_top(color, ui->lbl_color_display);
		
		//set color_bars

		//rrrgggbbb
		set_color_bar(color, ui->wdgt_rrrgggbbb_display_r, "red");
		set_color_bar(color, ui->wdgt_rrrgggbbb_display_g, "green");
		set_color_bar(color, ui->wdgt_rrrgggbbb_display_b, "blue");
		//rgbrgbrgb
		set_color_bar(color, ui->wdgt_rgbrgbrgb_display_r, "red");
		set_color_bar(color, ui->wdgt_rgbrgbrgb_display_g, "green");
		set_color_bar(color, ui->wdgt_rgbrgbrgb_display_b, "blue");

		//lbl colors
		set_color_label(color, ui->lbl_display_r, "red");
		set_color_label(color, ui->lbl_display_g, "green");
		set_color_label(color, ui->lbl_display_b, "blue");
	}
};

//set_color_bar
void Inspect_view::set_color_bar(QColor& color,  QWidget*& wdgt_color_bar, std::string color_type)
{
	//color_value
	std::string color_value;
	//stylesheet
	boost::format stylesheet;
	//size_value
	float size_value = 0;

	//assign based on color_type
	//red
	if(color_type.compare("red") == 0)
	{
		color_value = boost::lexical_cast<std::string>(color.red());
		stylesheet = boost::format("background-color: rgb(%s, 0, 0)");
		size_value = static_cast<float>(color.red()) * COLORBAR_SCALEFACTOR;
	}
	//green
	else if(color_type.compare("green") == 0)
	{
		color_value = boost::lexical_cast<std::string>(color.green());
		stylesheet = boost::format("background-color: rgb(0, %s, 0)");
		size_value = static_cast<float>(color.green()) * COLORBAR_SCALEFACTOR;
	}
	//blue
	else if(color_type.compare("blue") == 0)
	{
		color_value = boost::lexical_cast<std::string>(color.blue());
		stylesheet = boost::format("background-color: rgb(0, 0, %s)");
		size_value = static_cast<float>(color.blue()) * COLORBAR_SCALEFACTOR;
	}

	
	//format stylesheet
	stylesheet % color_value;
	
	//set stylesheet
	wdgt_color_bar->setStyleSheet(QString(stylesheet.str().c_str()));

	//set size
	wdgt_color_bar->setMinimumHeight(static_cast<int>(size_value));

};

//set_color_label
void Inspect_view::set_color_label(QColor& color, QLabel*& wdgt_color_label, std::string color_type)
{
	//color_value
	std::string color_value;
	//stylesheet
	boost::format stylesheet;

	//assign based on color_type
	//red
	if(color_type.compare("red") == 0)
	{
		color_value = boost::lexical_cast<std::string>(color.red());
		stylesheet = boost::format("color: rgb(%s, 0, 0);");
	}
	//green
	else if(color_type.compare("green") == 0)
	{
		color_value = boost::lexical_cast<std::string>(color.green());
		stylesheet = boost::format("color: rgb(0, %s, 0);");
	}
	//blue
	else if(color_type.compare("blue") == 0)
	{
		color_value = boost::lexical_cast<std::string>(color.blue());
		stylesheet = boost::format("color: rgb(0, 0, %s);");
	};

	//format stylesheet
	stylesheet % color_value;

	//set label text
	wdgt_color_label->setText(color_value.c_str());
	//set stylesheet
	wdgt_color_label->setStyleSheet(QString(stylesheet.str().c_str()));
};

//set_color_label_top
void Inspect_view::set_color_label_top(QColor& color, QLabel*& wdgt_color_label)
{
	//color_value
	std::string color_value;
	//stylesheet
	boost::format stylesheet("background-color: rgb(%s, %s, %s);");
	stylesheet % color.red() % color.green() % color.blue();
	
	//set stylesheet
	wdgt_color_label->setStyleSheet(QString(stylesheet.str().c_str()));
};