#include "Textep.h"

VMWCHAR file_pathw[101];
VMWCHAR file_pathq[101];
VMWCHAR title_name[101];
VMFILE f_read;
VMINT trigerisX = 0;

VMUINT8 *buffer;
VMINT trigeris = 1;
VMINT trigeris1 = 0;
VMINT trigeris2 = 0;
VMINT trigeris3 = 0;
VMINT trigeris4 = 0;

VMWCHAR tekstas[2000];
VMWCHAR auto_filename[101];

void vm_main(void) {

    layer_hdl[0] = -1;
    vm_reg_sysevt_callback(handle_sysevt);
    vm_font_set_font_size(VM_SMALL_FONT);
    checkFileExist();

}

void handle_sysevt(VMINT message, VMINT param) {

    switch (message) {
        case VM_MSG_CREATE:
        case VM_MSG_ACTIVE:
            layer_hdl[0] =
                vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(),
                                        vm_graphic_get_screen_height(), -1);
            vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(),
                                vm_graphic_get_screen_height());
            buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
            vm_switch_power_saving_mode(turn_off_mode);
            break;

        case VM_MSG_PAINT:
            vm_switch_power_saving_mode(turn_off_mode);
            if (trigeris1 == 1) {trigeris2 = 1;}
            if (trigeris == 0) {trigeris1 = 1;}
            if (trigeris2 == 1 && trigeris3 == 0) {
               trigeris3 = 1;
               create_auto_filename(title_name);
               create_auto_full_path_name(file_pathw, title_name);
               vm_input_set_editor_title((VMWSTR)title_name);
               vm_input_text3(NULL, 2000, 32, save_text);}
            else if (trigeris4 == 1){
               read_file_to_input(file_pathw);
               trigeris4 = 0;
            }
            else{}
            break;

        case VM_MSG_INACTIVE:

            vm_switch_power_saving_mode(turn_on_mode);
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;

        case VM_MSG_QUIT:
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;
    }
}

void checkFileExist(void) {

    VMWCHAR appTxtName[100];

    create_app_txt_filename(appTxtName);
    create_auto_full_path_name(file_pathw, appTxtName);
  
    wstrcpy(title_name, appTxtName);
    vm_input_set_editor_title((VMWSTR)title_name);

    f_read = vm_file_open(file_pathw, MODE_READ, FALSE);

    if (f_read < 0) {
        vm_file_close(f_read);
        trigeris = vm_selector_run(0, 0, file);
    } else {
        vm_file_close(f_read);
        read_file_to_input(file_pathw);
       
    }
    return;
}

void save_text(VMINT state, VMWSTR text) {

    VMCHAR new_data[2000];
    VMUINT nwrite;
    int lenght;
    VMWCHAR wAutoFileNamez[101];
    VMWCHAR wAutoFullPathNamez[101];

    lenght = wstrlen(text);

    if (state == VM_INPUT_OK) {
        if (trigerisX == 1) {
            create_auto_filename(wAutoFileNamez);
            create_auto_full_path_name(wAutoFullPathNamez, wAutoFileNamez);
            vm_file_rename((VMWSTR)file_pathw, (VMWSTR)wAutoFullPathNamez);
        }

        if (lenght > 0) {
           f_read = vm_file_open(file_pathw, MODE_CREATE_ALWAYS_WRITE, FALSE);
           vm_chset_convert(VM_CHSET_UCS2, VM_CHSET_UTF8, (VMCHAR *)text, new_data, (lenght + 1) * 2); //src, dst
           //vm_ucs2_to_ascii(new_data, 2000, (VMWSTR)text); //+
           vm_file_write(f_read, new_data, strlen(new_data), &nwrite);
           vm_file_close(f_read);
        }
    }
    vm_exit_app();
}

VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    trigeris = 1;
    trigeris1 = 0;
    trigeris2 = 0;
    //trigeris3 = 0;
    VMWCHAR appName[100];

    vm_get_filename(FILE_PATH, appName);
    wstrcpy(title_name, appName);
    vm_input_set_editor_title((VMWSTR)title_name);
    vm_wstrcpy(file_pathw, FILE_PATH);
    trigeris4 = 1;
    return 0;
}

void create_auto_filename(VMWSTR text) {

    struct vm_time_t curr_time;
    VMCHAR fAutoFileName[100];

    vm_get_time(&curr_time);
    sprintf(fAutoFileName, "%02d%02d%02d%02d%02d.txt", curr_time.mon, curr_time.day, curr_time.hour, curr_time.min, curr_time.sec);
    vm_ascii_to_ucs2(text, (strlen(fAutoFileName) + 1) * 2, fAutoFileName);

}

void create_auto_full_path_name(VMWSTR result, VMWSTR fname) {

    VMINT drv;
    VMCHAR fAutoFileName[100];
    VMWCHAR wAutoFileName[100];

    if ((drv = vm_get_removable_driver()) < 0) {
       drv = vm_get_system_driver();
    }

    sprintf(fAutoFileName, "%c:\\", drv);
    vm_ascii_to_ucs2(wAutoFileName, (strlen(fAutoFileName) + 1) * 2, fAutoFileName);
    vm_wstrcat(wAutoFileName, fname);
    vm_wstrcpy(result, wAutoFileName);

}

void create_app_txt_filename(VMWSTR text) {

    VMWCHAR fullPath[100];
    VMWCHAR appName[100];
    VMCHAR asciiAppName[100];
    VMCHAR file_name[100];

    vm_get_exec_filename(fullPath);
    vm_get_filename(fullPath, appName);
    vm_ucs2_to_ascii(asciiAppName, wstrlen(appName) + 1, appName);
    memcpy(file_name, asciiAppName, strlen(asciiAppName) - 3);
    file_name[strlen(asciiAppName) - 3] = '\0';
    strcat(file_name, "txt");
    vm_ascii_to_ucs2(text, (strlen(file_name) + 1) * 2, file_name);

}

void read_file_to_input(VMWSTR path) {

   VMCHAR ascii_data[2000];
   VMCHAR UTF8_data[2000];
   VMCHAR ucs2_data[2000];  // ucs2 turi buti VMWCHAR tipo !!!!
   VMUINT nread;

   f_read = vm_file_open(path, MODE_READ, FALSE);
   vm_file_read(f_read, ascii_data, 2149, &nread); //ar utf8 koduotes tekstas bus VMCHAR tipo ??????????
   ascii_data[nread] = '\0';
   vm_file_close(f_read);
   vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, ascii_data, ucs2_data, (strlen(ascii_data) + 1) *2);
   //vm_ascii_to_ucs2((VMWSTR)ucs2_data, (strlen(ascii_data) + 1) * 2, ascii_data); //+
   trigerisX = 1;
   vm_input_text3((VMWSTR)ucs2_data, 2000, 32, save_text);
}