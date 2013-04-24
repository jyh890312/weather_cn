#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk-3.0/gtk/gtk.h>
#include "wether.h"

GtkBuilder *builder;
static GtkWidget *window;
GtkWidget *event_box;
GtkWidget *show_right_event_box;
GtkWidget *close_label;
GtkWidget *min_label;
GtkWidget *update_time_label;

GtkWidget *city_label;
GtkWidget *time_label;
GtkWidget *main_wether_image;
GtkWidget *temperature_label;
GtkWidget *humidity_label;

GtkWidget *wind_label;
GtkWidget *air_quality_label;
GtkWidget *UV_intensity_label;

GtkArrow *show_right_arrow;

static bool is_show_right = FALSE;

gint window_button_press_event(GtkWidget * widget,
          GdkEventButton * event, gpointer data)
{
	if (event->button == 1)       // 判断是否左键按下
    {
		gtk_window_begin_move_drag(
					GTK_WINDOW(gtk_widget_get_toplevel(widget)),
					event->button,event->x_root,event->y_root,event->time);
	}
	return TRUE;
}

gint show_right_event(GtkWidget * widget,
          GdkEventButton * event, gpointer data)
{
	gtk_window_set_resizable(window,TRUE);
	if(TRUE == is_show_right){
		gtk_widget_set_size_request(GTK_WINDOW(window),250,300);
		is_show_right = FALSE;
	}else{
		gtk_widget_set_size_request(GTK_WINDOW(window),500,300);
		is_show_right = TRUE;
	}
	gtk_window_set_resizable(window,FALSE);
	return TRUE;
}


void init_main_window(){
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window")); //获取并生成界面对象
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	
	//去掉标题栏
	gtk_window_set_resizable(window,TRUE);
	gtk_widget_set_size_request(GTK_WINDOW(window),250,300);
	gtk_window_set_resizable(window,FALSE);
	gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

	GdkRGBA color;
	color.red = 255.0/255.0;
	color.green = 194.0/255.0;
	color.blue = 47.0/255.0;
	color.alpha = 1.0;
	gtk_widget_override_background_color(window,GTK_STATE_FLAG_NORMAL,
				&color);	

	gtk_widget_add_events(window,GDK_BUTTON_PRESS_MASK );
    g_signal_connect(G_OBJECT(window), "button_press_event",
                   G_CALLBACK(window_button_press_event), window);       // 加入事件回调
	//gtk_widget_show(window);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);

    gtk_builder_connect_signals(builder, NULL);
}

void init_title_bar(){

	event_box = GTK_WIDGET(gtk_builder_get_object(builder,"event_box"));
	//event_box = gtk_event_box_new();
	//gtk_container_add(GTK_CONTAINER(window),event_box);

	//*close_label;
	close_label = GTK_WIDGET(gtk_builder_get_object(builder,"close_label"));
	gtk_label_set_text((GtkLabel*)close_label,"X");
	gtk_label_set_markup(GTK_LABEL(close_label),
				"<span foreground='white' background='#B0D047' font_desc='20'>X</span>");
	//gtk_container_add(GTK_CONTAINER(event_box),close_label);
	g_signal_connect(G_OBJECT(event_box),"button-press-event",
				G_CALLBACK(gtk_main_quit),NULL);

	//*min_label;
	min_label= GTK_WIDGET(gtk_builder_get_object(builder,"min_label"));
	gtk_label_set_text((GtkLabel*)min_label,"-");
	gtk_label_set_markup(GTK_LABEL(min_label),
				"<span foreground='white' background='#B0D047' font_desc='20'>-</span>");

	//*show_right_event_box;
	show_right_event_box = GTK_WIDGET(gtk_builder_get_object(builder,"show_right_event_box"));
	g_signal_connect(G_OBJECT(show_right_event_box),"button-press-event",
				G_CALLBACK(show_right_event),NULL);

}

GtkWidget *create_main_window(){
	//GtkBuilder *builder;

    builder = gtk_builder_new();//创建GtkBuilder对象
    gtk_builder_add_from_file(builder, "ui/wether_gtk_fixed.glade", NULL);//读取glade文件

	init_main_window();
	init_title_bar();

    char wether_info[11][255];
	int wether_type = 0;
	getWebtoXmlFile("www.webxml.com.cn");
	paraseXmlWetherInfo(wether_info,&wether_type);

	int i;
	for(i=0;i<11;i++){
		printf("%s\n",wether_info+i);
	} 

	char wether_now_info[5][255];
	char *pch;
	pch = strtok(wether_info+10,"：；");
	int num = 0;//无奈之举
	while(NULL != pch){
		if(0 == strcmp(pch,"气温")){
		  strcpy(wether_now_info+0,pch = strtok(NULL,"：；"));
		}else if(3 == num){// if(0 == strcmp(pch,"风"/*"风向/风力"*/)){
		  strcpy(wether_now_info+1,pch = strtok(NULL,"：；"));
		}else if(0 == strcmp(pch,"湿度")){
		  strcpy(wether_now_info+2,pch = strtok(NULL,"：；"));
		}else if(0 == strcmp(pch,"空气质量")){
		  strcpy(wether_now_info+3,pch = strtok(NULL,"：；"));
		}else if(0 == strcmp(pch,"\272度"/*"紫外线强度"*/)){
		  strcpy(wether_now_info+4,pch = strtok(NULL,"：；"));
		}else{
			pch = strtok(NULL,"：；");
		}
		num++;
	}

	for(i=0;i<5;i++){
		printf("%s\n",wether_now_info+i);
	} 

	//*city_label
	char tmp_str[255];
	city_label = GTK_WIDGET(gtk_builder_get_object(builder,"city_label"));
	sprintf(tmp_str,"<span foreground='#4A4C4D' underline='double' font_desc='24'>%s</span>",wether_info+1);
	gtk_label_set_text((GtkLabel*)city_label,wether_info+1);
	gtk_label_set_markup(GTK_LABEL(city_label),tmp_str);
	//*time_label
	time_label = GTK_WIDGET(gtk_builder_get_object(builder,"time_label"));
	sprintf(tmp_str,"<span foreground='#4A4C4D' underline='single' font_desc='12'>%s</span>",wether_info+6);
	gtk_label_set_markup(GTK_LABEL(time_label),tmp_str);


	//*main_wether_image;
	main_wether_image = GTK_WIDGET(gtk_builder_get_object(builder,"main_wether_image"));
	char image_file_str[10];
	sprintf(image_file_str,"pic/%d.png",wether_type);
	gtk_image_set_from_file((GtkImage*)main_wether_image,image_file_str);

	//*temperature_label
	temperature_label = GTK_WIDGET(gtk_builder_get_object(builder,"temperature_label"));
	sprintf(tmp_str,"<span foreground='#4A4C4D' font_desc='20'>%s</span>",wether_now_info+0);
	gtk_label_set_markup(GTK_LABEL(temperature_label),tmp_str);
	gtk_widget_set_tooltip_markup(temperature_label,"<span foreground='white' font_desc='12'>此时温度</span>");

	//*humidity_label
	humidity_label= GTK_WIDGET(gtk_builder_get_object(builder,"humidity_label"));
	sprintf(tmp_str,"<span foreground='#4A4C4D' font_desc='20'>%s</span>",wether_now_info+2);
	gtk_label_set_markup(GTK_LABEL(humidity_label),tmp_str);
	gtk_widget_set_tooltip_markup(humidity_label,"<span foreground='white' font_desc='12'>空气湿度</span>");

	//*wind_label
	wind_label= GTK_WIDGET(gtk_builder_get_object(builder,"wind_label"));
	sprintf(tmp_str,"<span foreground='#4A4C4D' font_desc='12'>风力/风向：%s</span>",
				wether_now_info+1);
	gtk_label_set_markup(GTK_LABEL(wind_label),tmp_str);

	air_quality_label= GTK_WIDGET(gtk_builder_get_object(builder,"air_quality_label"));
	sprintf(tmp_str,"<span foreground='#4A4C4D' font_desc='12'>空气质量： %s</span>",
				wether_now_info+3);
	gtk_label_set_markup(GTK_LABEL(air_quality_label),tmp_str);

	UV_intensity_label = GTK_WIDGET(gtk_builder_get_object(builder,"UV_intensity_label"));
	sprintf(tmp_str,"<span foreground='#4A4C4D' font_desc='12'>紫外线强度：%s</span>",wether_now_info+4);
	gtk_label_set_markup(GTK_LABEL(UV_intensity_label),tmp_str);

	//*update_time_label
	update_time_label= GTK_WIDGET(gtk_builder_get_object(builder,"update_time_label"));
	sprintf(tmp_str,"<span foreground='#4A4C4D' font_desc='10'>%s</span>",wether_info+4);
	gtk_label_set_markup(GTK_LABEL(update_time_label),tmp_str);
	gtk_widget_set_tooltip_markup(update_time_label,"<span foreground='white' font_desc='12'>更新时间</span>");

	//8show_right_arrow
	show_right_arrow = GTK_WIDGET(gtk_builder_get_object(builder,"show_right_arrow"));
	g_signal_connect(G_OBJECT(show_right_arrow),"button-press-event",G_CALLBACK(gtk_main_quit),NULL);
    g_object_unref(G_OBJECT(builder));//释放GtkBuilder对象
    return window;
}

#endif // INTERFACE_H
