#include "Textep.h"

VMCHAR file_name[101] = "new.txt";
VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
VMFILE f_read;
VMINT drv;
VMINT trigeris = 0;

void vm_main(void) {

        checkFileExist();
}

void checkFileExist(void) {

    VMCHAR f_name[MRE_STR_SIZE_MAX + 1];
    int size = 0;

    if ((drv = vm_get_removable_driver()) < 0) {
        drv = vm_get_system_driver();
    }

    sprintf(f_name, "%c:\\%s", drv, file_name);
    vm_ascii_to_ucs2(f_wname, MRE_STR_SIZE_MAX, f_name);
    f_read = vm_file_open(f_wname, MODE_READ, FALSE);

    if (f_read < 0) {

        vm_file_close(f_read);
        vm_input_set_editor_title((VMWSTR)f_wname);
        vm_input_text3(NULL, 2000, 8, save_text);

    } else {

        VMCHAR file_data[2000];
        VMWSTR data_str[2000];
        VMUINT nread;
        vm_file_read(f_read, file_data, 2000, &nread);
        vm_ascii_to_ucs2((VMWSTR)data_str, 2000, file_data);
        vm_file_close(f_read);
        trigeris = 1;
        vm_input_set_editor_title((VMWSTR)f_wname);
        vm_input_text3((VMWSTR)data_str, 2000, 8, save_text);

        //vm_input_set_vk_default_enlarge();
        //vm_input_move_cursor_to_start();

    }
}

void save_text(VMINT state, VMWSTR text) {

    if (state == VM_INPUT_OK){

       if (trigeris == 1) {
          struct vm_time_t curr_time;
          VMWSTR fAutoFileName[MRE_STR_SIZE_MAX + 1];
          VMWCHAR wAutoFileName[MRE_STR_SIZE_MAX + 1];
          vm_get_time(&curr_time);
          sprintf(fAutoFileName, "%c:\\%d%d%d%d%d.txt", drv, curr_time.mon, curr_time.day, curr_time.hour, curr_time.min, curr_time.sec);
          vm_ascii_to_ucs2(wAutoFileName, 100, fAutoFileName);
          vm_file_rename((VMWSTR)f_wname, (VMWSTR)wAutoFileName);
       }

       VMCHAR new_data[2000];
       VMUINT nwrite;
       f_read = vm_file_open(f_wname, MODE_CREATE_ALWAYS_WRITE, FALSE);
       vm_ucs2_to_ascii(new_data, 2000, (VMWSTR)text);
       vm_file_write(f_read, new_data, strlen(new_data), &nwrite);
       vm_file_close(f_read);
    }
    vm_exit_app();
}
