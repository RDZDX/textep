#include "Textep.h"

VMUINT8 *buffer;
VMCHAR file_name[101] = "new.txt";
VMWSTR path;

VMWSTR path1[200];
VMCHAR data_text[2000];
VMWSTR data_str[2000];
VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
VMFILE f_read;

void vm_main(void) {

	layer_hdl[0] = -1;	
	vm_reg_sysevt_callback(handle_sysevt);
	vm_reg_keyboard_callback(handle_keyevt);
	vm_reg_pen_callback(handle_penevt);
        vm_font_set_font_size(VM_SMALL_FONT);
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
                //draw_black_rectangle();
                checkFileExist();
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
       //checkFileExist();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM1) {
       vm_audio_play_beep();
       //vm_vibrator_once();
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

void checkFileExist(void) {

    //draw_black_rectangle();

    VMINT drv;
    VMCHAR f_name[MRE_STR_SIZE_MAX + 1];
    //VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
    //VMFILE f_read;
    int size = 0;

    if ((drv = vm_get_removable_driver()) < 0) {
        drv = vm_get_system_driver();
    }

    sprintf(f_name, "%c:\\%s", drv, file_name);
    vm_ascii_to_ucs2(f_wname, MRE_STR_SIZE_MAX, f_name);
    f_read = vm_file_open(f_wname, MODE_READ, FALSE);

    if (f_read < 0) {

        vm_file_close(f_read);
        vm_graphic_reset_status_icon_bar();
        //vm_input_set_editor_title(path1); //path1 formatas VMWSTR !
        vm_input_text3((VMWSTR)data_str, 2000, 8, save_text);

    } else {

        VMUINT nread;
        vm_file_read(f_read, data_text, 2000, &nread);
        vm_ascii_to_ucs2((VMWSTR)data_str, 2000, data_text);

        vm_file_close(f_read);
        //vm_file_delete(f_wname);
        vm_graphic_reset_status_icon_bar();
        //vm_input_set_editor_title(path1); //path1 formatas VMWSTR !
        vm_input_text3((VMWSTR)data_str, 2000, 8, save_text);

    }
}

void save_text(VMINT state, VMWSTR text) {  //nuskaitytas tekstas ikeliamas i ivedimo formos langa

    if (state == VM_INPUT_OK){
       VMUINT nwrite;
       f_read = vm_file_open(f_wname, MODE_CREATE_ALWAYS_WRITE, FALSE);
       vm_ucs2_to_ascii(data_text, 2000, (VMWSTR)text);
       vm_file_write(f_read, data_text, strlen(data_text), &nwrite);
       vm_file_close(f_read);
    }
    vm_exit_app();
}

VMINT my_path(VMWSTR file_path, VMINT wlen) { //atidaromas failas, nuskaitomas ascii duomenu fragmentas(2000), fragm konvertuojamas
                                              //i UCS2, ucs2 duomenys ikeliami i data_str, failas uzdaromas, failas atidaromas ? 
    VMUINT nread;
    //data = vm_file_open(file_path, 1, FALSE);
    //vm_file_read(data, data_text, 2000, &nread);
    //vm_ascii_to_ucs2((VMWSTR)data_str, 2000, data_text);
    //vm_file_close(data);
    //data = vm_file_open(file_path, 4, FALSE);
}
