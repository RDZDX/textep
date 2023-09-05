#include "Textep.h"

VMCHAR file_name[100] = "new.txt";
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

        VMCHAR ascii_data[2149];
        VMCHAR ucs2_data[2150];
        VMUINT nread;
        vm_file_read(f_read, ascii_data, 2149, &nread);
        ascii_data[nread] = '\0';
        vm_file_close(f_read);
        vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, ascii_data, ucs2_data, (strlen(ascii_data) + 1) * 2);
        trigeris = 1;
        vm_input_text3((VMWSTR)ucs2_data, 2000, 8, save_text);
    }

}

void save_text(VMINT state, VMWSTR text) {

    if (state == VM_INPUT_OK) {
        if (trigeris == 1) {
            struct vm_time_t curr_time;
            VMCHAR fAutoFileName[100];
            VMWCHAR wAutoFileName[101];
            vm_get_time(&curr_time);
            sprintf(fAutoFileName, "%c:\\%d%d%d%d%d.txt", drv, curr_time.mon, curr_time.day, curr_time.hour, curr_time.min, curr_time.sec);
            vm_ascii_to_ucs2(wAutoFileName, 100, fAutoFileName);
            vm_file_rename((VMWSTR)file_pathw, (VMWSTR)wAutoFileName);
        }


        if (wstrlen(text) > 0) {
           VMCHAR new_data[2000];
           VMCHAR new_data1[2000];
           VMUINT nwrite;
           f_read = vm_file_open(file_pathw, MODE_CREATE_ALWAYS_WRITE, FALSE);
           vm_wstrcpy(new_data1, text);
           vm_chset_convert(VM_CHSET_UCS2, VM_CHSET_UTF8, new_data1, new_data, (strlen(new_data1) + 1) * 2);
           vm_file_write(f_read, new_data, strlen(new_data), &nwrite);
           new_data[nwrite] = '\0';
           vm_file_close(f_read);
        }
    }
    vm_exit_app();
}
