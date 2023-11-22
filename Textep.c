#include "Textep.h"

//VMCHAR file_name[100] = "new.txt";
VMCHAR file_name[100];
VMWCHAR file_pathw[101];
VMFILE f_read;
VMINT drv;
VMINT trigeris = 0;
VMWSTR data_str;

void vm_main(void) {

    checkFileExist();

}

void checkFileExist(void) {

    VMCHAR file_path[100];

    VMWCHAR fullPath[100];
    VMWCHAR appName[100];
    VMWCHAR newName[100];
    VMCHAR asciiAppName[100];
    //VMCHAR file_name[100];

    vm_get_exec_filename(fullPath);
    vm_get_filename(fullPath, appName);
    vm_ucs2_to_ascii(asciiAppName, 100, appName);
    memcpy(file_name, asciiAppName, strlen(asciiAppName) - 3);
    file_name[strlen(asciiAppName) - 3] = '\0';
    strcat(file_name, "txt");

    if ((drv = vm_get_removable_driver()) < 0) {
        drv = vm_get_system_driver();
    }

    sprintf(file_path, "%c:\\%s", drv, file_name);
    vm_ascii_to_ucs2(file_pathw, 100, file_path);
    vm_input_set_editor_title((VMWSTR)file_pathw);
    f_read = vm_file_open(file_pathw, MODE_READ, FALSE);

    if (f_read < 0) {
        vm_file_close(f_read);
        vm_input_text3(NULL, 2000, 8, save_text);
    } else {

        VMCHAR ascii_data[2000];
        VMCHAR UTF8_data[2000];
        VMCHAR ucs2_data[2000];  // ucs2 turi buti VMWCHAR tipo !!!!
        VMUINT nread;
        vm_file_read(f_read, ascii_data, 2149, &nread); //ar utf8 koduotes tekstas bus VMCHAR tipo ??????????
        ascii_data[nread] = '\0';
        vm_file_close(f_read);
        vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, ascii_data, ucs2_data, (strlen(ascii_data) + 1) *2);
        trigeris = 1;
        vm_input_text3((VMWSTR)ucs2_data, 2000, 8, save_text);
    }

}

void save_text(VMINT state, VMWSTR text) {

    VMCHAR new_data[2000];
    VMUINT nwrite;
    int lenght;

    lenght = wstrlen(text);

    if (state == VM_INPUT_OK) {
        if (trigeris == 1) {
            struct vm_time_t curr_time;
            VMCHAR fAutoFileName[100];
            VMWCHAR wAutoFileName[101];
            vm_get_time(&curr_time);
            //sprintf(fAutoFileName, "%c:\\%d%d%d%d%d.txt", drv, curr_time.mon, curr_time.day, curr_time.hour, curr_time.min, curr_time.sec);
            sprintf(fAutoFileName, "%c:\\%02d%02d%02d%02d%02d.txt", drv, curr_time.mon, curr_time.day, curr_time.hour, curr_time.min, curr_time.sec);
            vm_ascii_to_ucs2(wAutoFileName, 100, fAutoFileName);
            vm_file_rename((VMWSTR)file_pathw, (VMWSTR)wAutoFileName);
        }

        if (lenght > 0) {
           f_read = vm_file_open(file_pathw, MODE_CREATE_ALWAYS_WRITE, FALSE);
           //vm_chset_convert(VM_CHSET_UCS2, VM_CHSET_UTF8, text, new_data, lenght + 1); //src, dst
           vm_chset_convert(VM_CHSET_UCS2, VM_CHSET_UTF8, (VMCHAR *)text, new_data, (lenght + 1) * 2); //src, dst
           vm_file_write(f_read, new_data, strlen(new_data), &nwrite); //++++++++++++++++++++

           vm_file_close(f_read);
        }
    }
    vm_exit_app();
}
