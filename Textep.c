#include "Textep.h"

VMUINT8 *buffer;
VMCHAR file_name[101] = "new.txt";
VMWSTR path;

VMWSTR path1[200];
VMCHAR data_text[2000];
VMWSTR data_str[2000];

void vm_main(void) {

	layer_hdl[0] = -1;	
	vm_reg_sysevt_callback(handle_sysevt);
	vm_reg_keyboard_callback(handle_keyevt);
	vm_reg_pen_callback(handle_penevt);
        //vm_font_set_font_size(VM_SMALL_FONT);
        //checkFileExist();
}

void handle_sysevt(VMINT message, VMINT param) {

switch (message) {
	case VM_MSG_CREATE:
	case VM_MSG_ACTIVE:

		layer_hdl[0] = vm_graphic_create_layer(0, 0, 
			vm_graphic_get_screen_width(),
			vm_graphic_get_screen_height(),
			-1);
		
		vm_graphic_set_clip(0, 0, 
			vm_graphic_get_screen_width(), 
			vm_graphic_get_screen_height());
                buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
                vm_switch_power_saving_mode(turn_off_mode);
		break;
		
	case VM_MSG_PAINT:
                vm_switch_power_saving_mode(turn_off_mode);
		//draw_hello();
		break;
		
	case VM_MSG_INACTIVE:
                vm_switch_power_saving_mode(turn_on_mode);
		if( layer_hdl[0] != -1 )
			vm_graphic_delete_layer(layer_hdl[0]);
		
		break;	
	case VM_MSG_QUIT:
		if( layer_hdl[0] != -1 )
			vm_graphic_delete_layer(layer_hdl[0]);
                vm_free(path);

		break;	
	}
}

void handle_keyevt(VMINT event, VMINT keycode) {

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_RIGHT_SOFTKEY) {
        if (layer_hdl[0] != -1) {
            vm_graphic_delete_layer(layer_hdl[0]);
            layer_hdl[0] = -1;
        }
        vm_exit_app();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_LEFT_SOFTKEY) {
       vm_vibrator_once();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM1) {
       vm_audio_play_beep();
    }

}

void handle_penevt(VMINT event, VMINT x, VMINT y)
{
	if( layer_hdl[0] != -1 )
	{
		vm_graphic_delete_layer(layer_hdl[0]);
		layer_hdl[0] = -1;
	}
	
	vm_exit_app();
}

static void draw_hello(void) {

	VMWSTR s;
	int x;
	int y;
	int wstr_len;
	vm_graphic_color color;
	s = (VMWSTR)vm_res_get_string(STR_ID_HELLO);
	wstr_len = vm_graphic_get_string_width(s);
	x = (vm_graphic_get_screen_width() - wstr_len) / 2;
	y = (vm_graphic_get_screen_height() - vm_graphic_get_character_height()) / 2;
	color.vm_color_565 = VM_COLOR_WHITE;
	vm_graphic_setcolor(&color);
	vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
	color.vm_color_565 = VM_COLOR_BLUE;
	vm_graphic_setcolor(&color);
	vm_graphic_textout_to_layer(layer_hdl[0],x, y, s, wstr_len);
	vm_graphic_flush_layer(layer_hdl, 1);
}

void draw_black_rectangle(void) {

    filledDsplByLines = 0;
    vm_graphic_fill_rect(buffer, MRE_SCREEN_START_X, MRE_SCREEN_START_Y,
                         vm_graphic_get_screen_width(),
                         vm_graphic_get_screen_height(), VM_COLOR_BLACK,
                         VM_COLOR_BLACK);
    vm_graphic_flush_layer(layer_hdl, 1);
}

void checkFileExist(void) {

    draw_black_rectangle();

    VMINT drv;
    VMCHAR f_name[MRE_STR_SIZE_MAX + 1];
    VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
    VMFILE f_read;
    VMWCHAR *file_path = 0;
    int size = 0;

    if ((drv = vm_get_removable_driver()) < 0) {
        drv = vm_get_system_driver();
    }

    sprintf(f_name, "%c:\\%s", drv, file_name);
    vm_ascii_to_ucs2(f_wname, MRE_STR_SIZE_MAX, f_name);
    f_read = vm_file_open(f_wname, MODE_READ, FALSE);

    if (f_read < 0) {                                    //failo nera - file open for write/create

        vm_file_close(f_read);
        vm_graphic_reset_status_icon_bar();
        vm_input_set_editor_title(path1);
        vm_input_text3((VMWSTR)data_str, 2000, 8, text); //naudojama void text?

    } else {                                             //jei failas yra file open for write
        vm_file_close(f_read);
        size = (strlen(f_name) + 1) * 2;
        path = vm_malloc(size);
        vm_wstrcpy(path, f_wname);
        //uzkraunam duomenis is failo
        //atidarom ivedimo forma
        //i forma ikeliame duomenis

    }
}

void text(VMINT state, VMWSTR text) {  //nuskaitytas tekstas ikeliamas i ivedimo formos langa

    VMUINT nwrite;
    vm_ucs2_to_ascii(data_text, 2000, (VMWSTR)text);
    vm_ascii_to_ucs2((VMWSTR)data_str, 2000, data_text);
    vm_file_write(data, data_text, strlen(data_text), &nwrite);  //kur failo atidarymo procedura ?
    vm_file_close(data);

}

void name(VMINT state, VMWSTR text) { //is ivedimo formos teksto gaunamas naujo failo vardas ?

    VMUINT nread;
    VMCHAR path_text[200];
    VMCHAR text_ip[15];
    VMCHAR path_text2[200];
    vm_ucs2_to_ascii(path_text, 200, (VMWSTR)path1);
    vm_ucs2_to_ascii(path_text2, 200, text);
    strcat(path_text, path_text2);
    vm_ascii_to_ucs2((VMWSTR)path1, 150, path_text);
    data = vm_file_open((VMWSTR)path1, 4, FALSE); //atidaromas failas
}

VMINT my_path(VMWSTR file_path, VMINT wlen) { //atidaromas failas, nuskaitomas ascii duomenu fragmentas(2000), fragm konvertuojamas
                                              //i UCS2, ucs2 duomenys ikeliami i data_str, failas uzdaromas, failas atidaromas ? 
    VMUINT nread;
    data = vm_file_open(file_path, 1, FALSE);
    vm_file_read(data, data_text, 2000, &nread);
    vm_ascii_to_ucs2((VMWSTR)data_str, 2000, data_text);
    vm_file_close(data);
    data = vm_file_open(file_path, 4, FALSE);
}
